
#ifndef LUMBERYARD_EXPORT_H
#define LUMBERYARD_EXPORT_H

#ifdef LUMBERYARD_STATIC_DEFINE
#  define LUMBERYARD_EXPORT
#  define LUMBERYARD_NO_EXPORT
#else
#  ifndef LUMBERYARD_EXPORT
#    ifdef fmt_lumberyard_EXPORTS
        /* We are building this library */
#      define LUMBERYARD_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define LUMBERYARD_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef LUMBERYARD_NO_EXPORT
#    define LUMBERYARD_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef LUMBERYARD_DEPRECATED
#  define LUMBERYARD_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef LUMBERYARD_DEPRECATED_EXPORT
#  define LUMBERYARD_DEPRECATED_EXPORT LUMBERYARD_EXPORT LUMBERYARD_DEPRECATED
#endif

#ifndef LUMBERYARD_DEPRECATED_NO_EXPORT
#  define LUMBERYARD_DEPRECATED_NO_EXPORT LUMBERYARD_NO_EXPORT LUMBERYARD_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef LUMBERYARD_NO_DEPRECATED
#    define LUMBERYARD_NO_DEPRECATED
#  endif
#endif

#endif /* LUMBERYARD_EXPORT_H */
