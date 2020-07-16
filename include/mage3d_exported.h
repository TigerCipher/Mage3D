
#ifndef mage3d_EXPORT_H
#define mage3d_EXPORT_H

#ifdef MAGE3D_STATIC_DEFINE
#  define mage3d_EXPORT
#  define MAGE3D_NO_EXPORT
#else
#  ifndef mage3d_EXPORT
#    ifdef mage3d_EXPORTS
        /* We are building this library */
#      define mage3d_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define mage3d_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef MAGE3D_NO_EXPORT
#    define MAGE3D_NO_EXPORT 
#  endif
#endif

#ifndef MAGE3D_DEPRECATED
#  define MAGE3D_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MAGE3D_DEPRECATED_EXPORT
#  define MAGE3D_DEPRECATED_EXPORT mage3d_EXPORT MAGE3D_DEPRECATED
#endif

#ifndef MAGE3D_DEPRECATED_NO_EXPORT
#  define MAGE3D_DEPRECATED_NO_EXPORT MAGE3D_NO_EXPORT MAGE3D_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MAGE3D_NO_DEPRECATED
#    define MAGE3D_NO_DEPRECATED
#  endif
#endif

#endif /* mage3d_EXPORT_H */
