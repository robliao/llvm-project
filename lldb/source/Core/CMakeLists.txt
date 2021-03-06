lldb_tablegen(CoreProperties.inc -gen-lldb-property-defs
  SOURCE CoreProperties.td
  TARGET LLDBCorePropertiesGen)

lldb_tablegen(CorePropertiesEnum.inc -gen-lldb-property-enum-defs
  SOURCE CoreProperties.td
  TARGET LLDBCorePropertiesEnumGen)

set(LLDB_CURSES_LIBS)
set(LLDB_LIBEDIT_LIBS)

if (NOT LLDB_DISABLE_CURSES)
  list(APPEND LLDB_CURSES_LIBS ${CURSES_LIBRARIES})
  if(LLVM_ENABLE_TERMINFO AND HAVE_TERMINFO)
    list(APPEND LLDB_CURSES_LIBS ${TERMINFO_LIBS})
  endif()
  if (LLVM_BUILD_STATIC)
    list(APPEND LLDB_CURSES_LIBS gpm)
  endif()
endif()

add_lldb_library(lldbCore
  Address.cpp
  AddressRange.cpp
  AddressResolver.cpp
  AddressResolverFileLine.cpp
  AddressResolverName.cpp
  Communication.cpp
  Debugger.cpp
  Disassembler.cpp
  DumpDataExtractor.cpp
  DumpRegisterValue.cpp
  DynamicLoader.cpp
  EmulateInstruction.cpp
  FileLineResolver.cpp
  FileSpecList.cpp
  FormatEntity.cpp
  Highlighter.cpp
  IOHandler.cpp
  IOHandlerCursesGUI.cpp
  Mangled.cpp
  Module.cpp
  ModuleChild.cpp
  ModuleList.cpp
  Opcode.cpp
  PluginManager.cpp
  RichManglingContext.cpp
  SearchFilter.cpp
  Section.cpp
  SourceManager.cpp
  StreamAsynchronousIO.cpp
  StreamFile.cpp
  UserSettingsController.cpp
  Value.cpp
  ValueObject.cpp
  ValueObjectCast.cpp
  ValueObjectChild.cpp
  ValueObjectConstResult.cpp
  ValueObjectConstResultCast.cpp
  ValueObjectConstResultChild.cpp
  ValueObjectConstResultImpl.cpp
  ValueObjectDynamicValue.cpp
  ValueObjectList.cpp
  ValueObjectMemory.cpp
  ValueObjectRegister.cpp
  ValueObjectSyntheticFilter.cpp
  ValueObjectVariable.cpp

  DEPENDS
    clang-tablegen-targets

  LINK_LIBS
    lldbBreakpoint
    lldbDataFormatters
    lldbExpression
    lldbHost
    lldbInterpreter
    lldbSymbol
    lldbTarget
    lldbUtility
    lldbPluginCPlusPlusLanguage
    lldbPluginObjCLanguage
    ${LLDB_CURSES_LIBS}

  CLANG_LIBS
    clangDriver

  LINK_COMPONENTS
    Support
    Demangle
  )

add_dependencies(lldbCore
  LLDBCorePropertiesGen
  LLDBCorePropertiesEnumGen)

# Needed to properly resolve references in a debug build.
# TODO: Remove once we have better layering
set_target_properties(lldbCore PROPERTIES LINK_INTERFACE_MULTIPLICITY 4)

if (NOT LLDB_DISABLE_LIBEDIT)
  target_include_directories(lldbCore PRIVATE ${libedit_INCLUDE_DIRS})
endif()
