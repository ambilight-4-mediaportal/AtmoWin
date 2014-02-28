

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Sep 14 18:44:14 2009
 */
/* Compiler settings for .\RGBFilters.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __iRGBFilters_h__
#define __iRGBFilters_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __TransSmpte_FWD_DEFINED__
#define __TransSmpte_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransSmpte TransSmpte;
#else
typedef struct TransSmpte TransSmpte;
#endif /* __cplusplus */

#endif 	/* __TransSmpte_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __RGBFiltersLib_LIBRARY_DEFINED__
#define __RGBFiltersLib_LIBRARY_DEFINED__

/* library RGBFiltersLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_RGBFiltersLib;

EXTERN_C const CLSID CLSID_TransSmpte;

#ifdef __cplusplus

class DECLSPEC_UUID("345446AC-B638-40b4-BBE5-4098028AA14B")
TransSmpte;
#endif
#endif /* __RGBFiltersLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


