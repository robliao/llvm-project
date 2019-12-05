#include "UnderscoreLeadingCheck.h"
#include <algorithm>
#include "../utils/OptionsUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/CharInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/Regex.h"

#include <stdio.h>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace objc {

namespace {

std::string TransformTrailingToLeading(std::string input) {
  return "_" + input.substr(0, input.size() - 1);
}

AST_MATCHER_P(ObjCIvarRefExpr, hasDeclName, std::string, RegExp) {
  assert(!RegExp.empty());
  std::string FullNameString = "::" +
      Node.getDecl()->getQualifiedNameAsString();
  llvm::Regex RE(RegExp);
  return RE.match(FullNameString);
}

AST_MATCHER_P(ObjCPropertyImplDecl, hasPropertyDecl, std::string, RegExp) {
  assert(!RegExp.empty());
  const auto* IvarDecl = Node.getPropertyIvarDecl();
  if (!IvarDecl)
    return false;

  // If the Ivar was dynamically generated, let the Ivar matcher handle it.
  if (IvarDecl->getSourceRange() == Node.getPropertyIvarDeclLoc())
    return false;

  std::string FullNameString = "::" + IvarDecl->getQualifiedNameAsString();
  llvm::Regex RE(RegExp);
  return RE.match(FullNameString);
}

const ast_matchers::internal::
    VariadicDynCastAllOfMatcher<Decl, ObjCPropertyImplDecl>
    objcPropertyImplDecl;

}

void UnderscoreLeadingCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().ObjC)
    return;

  constexpr char IvarRegExp[] = "[a-zA-Z0-9_]+_$";
  Finder->addMatcher(
      objcIvarDecl(matchesName(IvarRegExp)).bind("ivar"), this);
  Finder->addMatcher(
      objcIvarRefExpr(hasDeclName(IvarRegExp)).bind("ivarref"), this);
  Finder->addMatcher(
      objcPropertyImplDecl(hasPropertyDecl(IvarRegExp)).bind("ivarpropdecl"),
      this);
}

void UnderscoreLeadingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<ObjCIvarDecl>("ivar");
  const auto *MatchedDeclRef =
      Result.Nodes.getNodeAs<ObjCIvarRefExpr>("ivarref");
  const auto *MatchedPropDecl =
      Result.Nodes.getNodeAs<ObjCPropertyImplDecl>("ivarpropdecl");

  if (MatchedDecl) {
    handleDecl(MatchedDecl);
  } else if (MatchedDeclRef) {
    handleExpr(MatchedDeclRef);
  } else if (MatchedPropDecl) {
    handlePropertyImplDecl(MatchedPropDecl);
  }
}

void UnderscoreLeadingCheck::handleDecl(const ObjCIvarDecl* Decl) {
  std::string NameWithTrailingUnderscore = Decl->getName();
  assert(NameWithTrailingUnderscore.size() > 0);

  std::string NameWithLeadingUnderscore =
      TransformTrailingToLeading(NameWithTrailingUnderscore);

  diag(Decl->getLocation(),
       "instance var name '%0' has a trailing underscore instead of a "
       "leading underscore.")
      << NameWithTrailingUnderscore
      << FixItHint::CreateReplacement(
             CharSourceRange::getTokenRange(
                 SourceRange(Decl->getLocation())),
             llvm::StringRef(NameWithLeadingUnderscore));
}

void UnderscoreLeadingCheck::handleExpr(const ObjCIvarRefExpr* Expr) {
  std::string NameWithTrailingUnderscore = Expr->getDecl()->getName();
  assert(NameWithTrailingUnderscore.size() > 0);

  std::string NameWithLeadingUnderscore =
      TransformTrailingToLeading(NameWithTrailingUnderscore);

  diag(Expr->getLocation(),
       "instance var reference '%0' has a trailing underscore instead of a "
       "leading underscore.")
      << NameWithTrailingUnderscore
      << FixItHint::CreateReplacement(
             CharSourceRange::getTokenRange(SourceRange(Expr->getLocation())),
             llvm::StringRef(NameWithLeadingUnderscore));
}

void UnderscoreLeadingCheck::handlePropertyImplDecl(
    const ObjCPropertyImplDecl* Decl) {
  std::string NameWithTrailingUnderscore =
      Decl->getPropertyIvarDecl()->getName();
  assert(NameWithTrailingUnderscore.size() > 0);

  std::string NameWithLeadingUnderscore =
      TransformTrailingToLeading(NameWithTrailingUnderscore);

  diag(Decl->getPropertyIvarDeclLoc(),
       "instance var property reference '%0' has a trailing underscore instead "
       "of a leading underscore.")
      << NameWithTrailingUnderscore
      << FixItHint::CreateReplacement(
             CharSourceRange::getTokenRange(
                 SourceRange(Decl->getPropertyIvarDeclLoc())),
             llvm::StringRef(NameWithLeadingUnderscore));
}

}  // namespace objc
}  // namespace tidy
}  // namespace clang
