#ifndef MYLIB_COMMON_H
#define MYLIB_COMMON_H

#if defined(MYLIB_BEGIN_C_DECLS)
#   undef MYLIB_BEGIN_C_DECLS
#endif
#if defined(MYLIB_END_C_DECLS)
#   undef MYLIB_END_C_DECLS
#endif
#if defined(MYLIB_C_DECL)
#   undef MYLIB_C_DECL
#endif
#if defined(__cplusplus)
#   define MYLIB_BEGIN_C_DECLS extern "C" {
#   define MYLIB_END_C_DECLS }
#   define MYLIB_C_DECL extern "C"
#else
#   define MYLIB_BEGIN_C_DECLS
#   define MYLIB_END_C_DECLS
#   define MYLIB_C_DECL
#endif

#endif // MYLIB_COMMON_H
