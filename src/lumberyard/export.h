
#ifndef DRAGON_EXPORT_H
#define DRAGON_EXPORT_H

#ifdef DRAGON_STATIC_DEFINE
#  define DRAGON_EXPORT
#  define DRAGON_NO_EXPORT
#else
#  ifndef DRAGON_EXPORT
#    ifdef fmt_lumberyard_EXPORTS
        /* We are building this library */
#      define DRAGON_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define DRAGON_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef DRAGON_NO_EXPORT
#    define DRAGON_NO_EXPORT 
#  endif
#endif

#ifndef DRAGON_DEPRECATED
#  define DRAGON_DEPRECATED __declspec(deprecated)
#endif

#ifndef DRAGON_DEPRECATED_EXPORT
#  define DRAGON_DEPRECATED_EXPORT DRAGON_EXPORT DRAGON_DEPRECATED
#endif

#ifndef DRAGON_DEPRECATED_NO_EXPORT
#  define DRAGON_DEPRECATED_NO_EXPORT DRAGON_NO_EXPORT DRAGON_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef DRAGON_NO_DEPRECATED
#    define DRAGON_NO_DEPRECATED
#  endif
#endif

#endif /* DRAGON_EXPORT_H */
