// no guards - it should be possible to include this header several times

#define UTILS_PARAMS_MACRO_1(macro) macro(1)
#define UTILS_PARAMS_MACRO_2(macro) UTILS_PARAMS_MACRO_1(macro), macro(2)
#define UTILS_PARAMS_MACRO_3(macro) UTILS_PARAMS_MACRO_2(macro), macro(3)
#define UTILS_PARAMS_MACRO_4(macro) UTILS_PARAMS_MACRO_3(macro), macro(4)
#define UTILS_PARAMS_MACRO_5(macro) UTILS_PARAMS_MACRO_4(macro), macro(5)
#define UTILS_PARAMS_MACRO_6(macro) UTILS_PARAMS_MACRO_5(macro), macro(6)
#define UTILS_PARAMS_MACRO_7(macro) UTILS_PARAMS_MACRO_6(macro), macro(7)
#define UTILS_PARAMS_MACRO_8(macro) UTILS_PARAMS_MACRO_7(macro), macro(8)
#define UTILS_PARAMS_MACRO_9(macro) UTILS_PARAMS_MACRO_8(macro), macro(9)


UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_1)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_2)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_3)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_4)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_5)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_6)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_7)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_8)
UTILS_INVOKE_PARAMS_MACRO(UTILS_PARAMS_MACRO_9)

#undef UTILS_INVOKE_PARAMS_MACRO

#undef UTILS_PARAMS_MACRO_1
#undef UTILS_PARAMS_MACRO_2
#undef UTILS_PARAMS_MACRO_3
#undef UTILS_PARAMS_MACRO_4
#undef UTILS_PARAMS_MACRO_5
#undef UTILS_PARAMS_MACRO_6
#undef UTILS_PARAMS_MACRO_7
#undef UTILS_PARAMS_MACRO_8
#undef UTILS_PARAMS_MACRO_9