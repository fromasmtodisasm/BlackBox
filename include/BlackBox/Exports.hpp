
#ifndef BLACKBOX_EXPORT_H
#define BLACKBOX_EXPORT_H

#ifdef BLACKBOX_STATIC_DEFINE
#  define BLACKBOX_EXPORT
#  define BLACKBOX_NO_EXPORT
#else
#  ifndef BLACKBOX_EXPORT
#    ifdef BlackBox_EXPORTS
        /* We are building this library */
#      define BLACKBOX_EXPORT 
#    else
        /* We are using this library */
#      define BLACKBOX_EXPORT 
#    endif
#  endif

#  ifndef BLACKBOX_NO_EXPORT
#    define BLACKBOX_NO_EXPORT 
#  endif
#endif

#ifndef BLACKBOX_DEPRECATED
#  define BLACKBOX_DEPRECATED __declspec(deprecated)
#endif

#ifndef BLACKBOX_DEPRECATED_EXPORT
#  define BLACKBOX_DEPRECATED_EXPORT BLACKBOX_EXPORT BLACKBOX_DEPRECATED
#endif

#ifndef BLACKBOX_DEPRECATED_NO_EXPORT
#  define BLACKBOX_DEPRECATED_NO_EXPORT BLACKBOX_NO_EXPORT BLACKBOX_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef BLACKBOX_NO_DEPRECATED
#    define BLACKBOX_NO_DEPRECATED
#  endif
#endif

#endif /* BLACKBOX_EXPORT_H */
