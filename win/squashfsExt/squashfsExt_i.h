

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sun Feb 02 09:55:07 2020
 */
/* Compiler settings for squashfsExt.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __squashfsExt_i_h__
#define __squashfsExt_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IsqfsExt_FWD_DEFINED__
#define __IsqfsExt_FWD_DEFINED__
typedef interface IsqfsExt IsqfsExt;

#endif 	/* __IsqfsExt_FWD_DEFINED__ */


#ifndef __sqfsExt_FWD_DEFINED__
#define __sqfsExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class sqfsExt sqfsExt;
#else
typedef struct sqfsExt sqfsExt;
#endif /* __cplusplus */

#endif 	/* __sqfsExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IsqfsExt_INTERFACE_DEFINED__
#define __IsqfsExt_INTERFACE_DEFINED__

/* interface IsqfsExt */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IsqfsExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ed6efc4a-9ac5-4cfa-a786-91402383349b")
    IsqfsExt : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IsqfsExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IsqfsExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IsqfsExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IsqfsExt * This);
        
        END_INTERFACE
    } IsqfsExtVtbl;

    interface IsqfsExt
    {
        CONST_VTBL struct IsqfsExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IsqfsExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IsqfsExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IsqfsExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IsqfsExt_INTERFACE_DEFINED__ */



#ifndef __squashfsExtLib_LIBRARY_DEFINED__
#define __squashfsExtLib_LIBRARY_DEFINED__

/* library squashfsExtLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_squashfsExtLib;

EXTERN_C const CLSID CLSID_sqfsExt;

#ifdef __cplusplus

class DECLSPEC_UUID("eaca1da7-a6db-42b5-883e-6d4051dbef1c")
sqfsExt;
#endif
#endif /* __squashfsExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


