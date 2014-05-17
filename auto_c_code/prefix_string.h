#ifndef PREFIX_STRING_H
#define PREFIX_STRING_H



/* v850 toolchain uses a `_' prefix for all user symbols */
#ifndef MODULE_SYMBOL_PREFIX
#define MODULE_SYMBOL_PREFIX ""
#endif

/* For every exported symbol, place a struct in the __ksymtab section */
#define __EXPORT_SYMBOL(sym, sec)				\
 static const char __prefix_##sym[]			\
 __attribute__((section("__prefix_strings")))		\
 = MODULE_SYMBOL_PREFIX #sym; 	\

#define EXPORT_SYMBOL(sym)					\
 __EXPORT_SYMBOL(sym, "")


#define FUNCDEBUG(A) __prefix_##A








class prefix_string
{
public:
    prefix_string();
};

#endif // PREFIX_STRING_H
