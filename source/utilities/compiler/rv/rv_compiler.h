/*****************************************************************************
 *   Copyright(C)2009-2022 by VSF Team                                       *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 ****************************************************************************/

#ifndef __USE_RV_COMPILER_H_PART_1__
#define __USE_RV_COMPILER_H_PART_1__

/*============================ INCLUDES ======================================*/

#include "./rv_compiler_detect.h"

#endif /* end of __USE_RV_COMPILER_H_PART_1__ */

/*========================== Multiple-Entry Start ============================*/

#if __IS_COMPILER_IAR__
#   include <intrinsics.h>
#endif

#ifndef __VSF_HEADER_ONLY_SHOW_COMPILER_INFO__
#   include "./type.h"
#endif
#include "../__common/__compiler.h"

/*========================== Multiple-Entry End ==============================*/

#ifndef __USE_RV_COMPILER_H_PART_2__
#define __USE_RV_COMPILER_H_PART_2__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------  Start of section using anonymous unions  -------------- */
#if __IS_COMPILER_IAR__
  #pragma language=extended
#elif __IS_COMPILER_GCC__
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/*============================ MACROS ========================================*/

//! \brief 1 cycle nop operation
#ifndef NOP
    #define NOP()               __asm__ __volatile__ ("nop");
#endif


//! \brief none standard memory types
#if __IS_COMPILER_IAR__
#   define ROM_FLASH            _Pragma(__VSF_STR(location=".rom.flash")) const
#   define ROM_EEPROM           _Pragma(__VSF_STR(location=".rom.eeprom")) const
#   define NO_INIT              __no_init
#   define ROOT                 __root
#   define INLINE               inline
#   define NO_INLINE            __attribute__((noinline))
#   define ALWAYS_INLINE        inline __attribute__((always_inline))
#   define WEAK(...)            __weak
#   define RAMFUNC              __ramfunc
#   define __asm__              __asm
#   define __ALIGN(__N)         __attribute__((aligned (__N)))
#   define __AT_ADDR(__ADDR)    @ __ADDR
#   define __SECTION(__SEC)     __attribute__((section (__SEC)))
#   define __WEAK_ALIAS(__ORIGIN, __ALIAS) \
                                _Pragma(__VSF_STR(weak __ORIGIN=__ALIAS))
#   define PACKED               __attribute__((packed))
#   define UNALIGNED            __attribute__((packed))
#   define TRANSPARENT_UNION    __attribute__((transparent_union))
#   define __ALIGN_OF(...)      __ALIGNOF__(__VA_ARGS__)

#   define __IAR_STARTUP_DATA_INIT  __iar_data_init3

#   define __ISR(__VEC)       void __VEC(void)

#endif

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/

extern void vsf_stdio_init(void);

#ifdef __cplusplus
}
#endif

#endif /* end of __USE_RV_COMPILER_H_PART_2__ */
