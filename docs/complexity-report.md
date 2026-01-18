# Complexity Report

Generated: Sun Jan 18 10:06:00 +03 2026

## Complexity Analysis (cppcheck)

```
Checking /opt/lxl/c++/10kingdoms/src/AM.cpp ...
/opt/lxl/c++/10kingdoms/src/AM.cpp:113:2: information: Include file: "gettext.h" not found. [missingInclude]
#include "gettext.h"
 ^
Checking /opt/lxl/c++/10kingdoms/src/AM.cpp: DEBUG...
Checking /opt/lxl/c++/10kingdoms/src/AM.cpp: ENABLE_INTRO_VIDEO...
Checking /opt/lxl/c++/10kingdoms/src/AM.cpp: FRENCH;GERMAN;SPANISH...
Checking /opt/lxl/c++/10kingdoms/src/AM.cpp: HAVE_LIBCURL...
1/328 files checked 0% done
Checking /opt/lxl/c++/10kingdoms/src/CmdLine.cpp ...
/opt/lxl/c++/10kingdoms/src/CmdLine.cpp:0:0: information: Limiting analysis of branches. Use --check-level=exhaustive to analyze all branches. [normalCheckLevelMaxBranches]

^
2/328 files checked 0% done
Checking /opt/lxl/c++/10kingdoms/src/ConfigAdv.cpp ...
/opt/lxl/c++/10kingdoms/src/ConfigAdv.cpp:34:2: information: Include file: "gettext.h" not found. [missingInclude]
#include "gettext.h"
 ^
/opt/lxl/c++/10kingdoms/src/ConfigAdv.cpp:652:27: style: Parameter 'in' can be declared as pointer to const [constParameterPointer]
static int read_int(char *in, int *out)
                          ^
/opt/lxl/c++/10kingdoms/src/ConfigAdv.cpp:675:27: style: Parameter 'in' can be declared as pointer to const [constParameterPointer]
static int read_key(char *in, char **out, KeyEventMap *event)
                          ^
Checking /opt/lxl/c++/10kingdoms/src/ConfigAdv.cpp: DEBUG...
Checking /opt/lxl/c++/10kingdoms/src/ConfigAdv.cpp: DEV_VERSION...
3/328 files checked 0% done
Checking /opt/lxl/c++/10kingdoms/src/LocaleRes.cpp ...
/opt/lxl/c++/10kingdoms/src/LocaleRes.cpp:44:17: style: C-style pointer casting [cstyleCast]
  return putenv((char*)lc_name_str);
                ^
/opt/lxl/c++/10kingdoms/src/LocaleRes.cpp:50:16: style: C-style pointer casting [cstyleCast]
 return putenv((char*)lc_all_str);
               ^
Checking /opt/lxl/c++/10kingdoms/src/LocaleRes.cpp: ENABLE_NLS...
/opt/lxl/c++/10kingdoms/src/LocaleRes.cpp:168:15: style: C-style pointer casting [cstyleCast]
  localeRec = (LocaleRec*) dbLocale->read(i+1);
              ^
/opt/lxl/c++/10kingdoms/src/LocaleRes.cpp:154:13: style: The scope of the variable 'localeRec' can be reduced. [variableScope]
 LocaleRec *localeRec;
            ^
/opt/lxl/c++/10kingdoms/src/LocaleRes.cpp:222:9: style: The scope of the variable 'c' can be reduced. [variableScope]
 size_t c;
        ^
Checking /opt/lxl/c++/10kingdoms/src/LocaleRes.cpp: ENABLE_NLS;HAVE_LC_MESSAGES...
Checking /opt/lxl/c++/10kingdoms/src/LocaleRes.cpp: LOCALE_DIR...
4/328 files checked 0% done
Checking /opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp ...
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:0:0: information: Limiting analysis of branches. Use --check-level=exhaustive to analyze all branches. [normalCheckLevelMaxBranches]

^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:703:26: warning: Potentially invalid type conversion in old-style C cast, clarify/fix with C++ cast [dangerousTypeCast]
   UnitMarine *shipPtr = (UnitMarine*) unitPtr;
                         ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:57:14: style: The scope of the variable 'actionNode' can be reduced. [variableScope]
 ActionNode* actionNode;
             ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:419:14: style: The scope of the variable 'actionNode' can be reduced. [variableScope]
 ActionNode* actionNode;
             ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:469:14: style: The scope of the variable 'actionNode' can be reduced. [variableScope]
 ActionNode* actionNode;
             ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:508:14: style: The scope of the variable 'actionNode' can be reduced. [variableScope]
 ActionNode* actionNode;
             ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:419:14: style: Variable 'actionNode' can be declared as pointer to const [constVariablePointer]
 ActionNode* actionNode;
             ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT.cpp:703:16: style: Variable 'shipPtr' can be declared as pointer to const [constVariablePointer]
   UnitMarine *shipPtr = (UnitMarine*) unitPtr;
               ^
5/328 files checked 1% done
Checking /opt/lxl/c++/10kingdoms/src/OAI_ACT2.cpp ...
/opt/lxl/c++/10kingdoms/src/OAI_ACT2.cpp:271:17: style: Variable 'unitRecno' is assigned a value that is never used. [unreadVariable]
 int   unitRecno=0;
                ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT2.cpp:322:21: style: Variable 'firmInfo' is assigned a value that is never used. [unreadVariable]
 FirmInfo* firmInfo = firm_res[firmId];
                    ^
/opt/lxl/c++/10kingdoms/src/OAI_ACT2.cpp:322:12: style: Variable 'firmInfo' is assigned a value that is never used. [unreadVariable]
 FirmInfo* firmInfo = firm_res[firmId];
           ^
6/328 files checked 1% done
Checking /opt/lxl/c++/10kingdoms/src/OAI_ATTK.cpp ...
/opt/lxl/c++/10kingdoms/src/OAI_ATTK.cpp:0:0: information: Limiting analysis of branches. Use --check-level=exhaustive to analyze all branches. [normalCheckLevelMaxBranches]

^
/opt/lxl/c++/10kingdoms/src/OAI_ATTK.cpp:73:6: style: The if condition is the same as the previous if condition [duplicateCondition]
 if( defenseMode )  // only for defense mode, for attack mission, we should plan and organize it better
     ^
/opt/lxl/c++/10kingdoms/src/OAI_ATTK.cpp:70:6: note: First condition
 if( defenseMode )
     ^
/opt/lxl/c++/10kingdoms/src/OAI_ATTK.cpp:73:6: note: Second condition
 if( defenseMode )  // only for defense mode, for attack mission, we should plan and organize it better
     ^
/opt/lxl/c++/10kingdoms/src/OAI_ATTK.cpp:133:56: style: C-style pointer casting [cstyleCast]
  attack_camp_array[attack_camp_count].combat_level = ((FirmCamp*)firm_array[leadAttackCampRecno])->total_combat_level();
                                                       ^
```

