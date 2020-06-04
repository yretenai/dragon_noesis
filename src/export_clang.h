
#ifndef DRAGON_EXPORT_H
#define DRAGON_EXPORT_H

#ifdef DRAGON_STATIC_DEFINE
#  define DRAGON_EXPORT
#  define DRAGON_NO_EXPORT
#else
#  ifndef DRAGON_EXPORT
#    ifdef fmt_dragon_EXPORTS
        /* We are building this library */
#      define DRAGON_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DRAGON_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DRAGON_NO_EXPORT
#    define DRAGON_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DRAGON_DEPRECATED
#  define DRAGON_DEPRECATED __attribute__ ((__deprecated__))
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
