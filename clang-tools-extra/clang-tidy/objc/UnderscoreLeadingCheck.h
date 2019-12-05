#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_OBJC_UNDERSCORE_LEADING_CHCEK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_OBJC_UNDERSCORE_LEADING_CHCEK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace objc {

class UnderscoreLeadingCheck : public ClangTidyCheck {
 public:
  UnderscoreLeadingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

 private:
  void handleDecl(const ObjCIvarDecl* Decl);
  void handleExpr(const ObjCIvarRefExpr* Expr);
  void handlePropertyImplDecl(const ObjCPropertyImplDecl* Decl);
};

} // namespace objc
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_OBJC_UNDERSCORE_LEADING_CHCEK_H
