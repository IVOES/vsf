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

#if VSF_HAL_USE_DAC == ENABLED

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/


#define vsf_real_dac_t                                          VSF_MCONNECT(VSF_DAC_CFG_IMP_PREFIX, _dac_t)
#define vsf_real_dac_capability                                 VSF_MCONNECT(VSF_DAC_CFG_IMP_PREFIX, _dac_capability)

#ifndef VSF_DAC_CFG_REIMPLEMENT_API_CAPABILITY
#   define  VSF_DAC_CFG_REIMPLEMENT_API_CAPABILITY              DISALBED
#endif

#if VSF_DAC_CFG_REIMPLEMENT_API_CAPABILITY == ENABLED
#   ifndef VSF_DAC_CFG_CAPABILITY_IRQ_MASK
#       define VSF_DAC_CFG_CAPABILITY_IRQ_MASK                  VSF_DAC_IRQ_ALL_BITS_MASK
#   endif
#   ifndef VSF_DAC_CFG_CAPABILITY_MAX_RESOLUTION_BITS
#       define VSF_DAC_CFG_CAPABILITY_MAX_RESOLUTION_BITS       12
#   endif
#   ifndef VSF_DAC_CFG_CAPABILITY_MIN_RESOLUTION_BITS
#       define VSF_DAC_CFG_CAPABILITY_MIN_RESOLUTION_BITS       8
#   endif
#   ifndef VSF_DAC_CFG_CAPABILITY_CHANNEL_COUNT
#       define VSF_DAC_CFG_CAPABILITY_CHANNEL_COUNT             4
#   endif
#endif

/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

#if VSF_DAC_CFG_REIMPLEMENT_API_CAPABILITY == ENABLED
vsf_dac_capability_t vsf_real_dac_capability(vsf_real_dac_t *dac_ptr)
{
    vsf_dac_capability_t dac_capability = {
        .irq_mask               = VSF_DAC_CFG_CAPABILITY_IRQ_MASK,
        .max_resolution_bits    = VSF_DAC_CFG_CAPABILITY_MAX_RESOLUTION_BITS,
        .min_resolution_bits    = VSF_DAC_CFG_CAPABILITY_MIN_RESOLUTION_BITS,
        .channel_count          = VSF_DAC_CFG_CAPABILITY_CHANNEL_COUNT,
    };

    return dac_capability;
}
#endif

/*============================ MACROS ========================================*/

#undef VSF_DAC_CFG_REIMPLEMENT_API_CAPABILITY
#undef VSF_DAC_CFG_CAPABILITY_IRQ_MASK
#undef VSF_DAC_CFG_CAPABILITY_MAX_RESOLUTION_BITS
#undef VSF_DAC_CFG_CAPABILITY_MIN_RESOLUTION_BITS
#undef VSF_DAC_CFG_CAPABILITY_CHANNEL_COUNT

#undef vsf_real_dac_t
#undef vsf_real_dac_capability

/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
/*============================ MACROS ========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

#define VSF_HAL_TEMPLATE_IMP_NAME                     _dac
#define VSF_HAL_TEMPLATE_IMP_UPCASE_NAME              _DAC

#ifndef VSF_DAC_CFG_IMP_PREFIX
#   error "Please define VSF_DAC_CFG_IMP_PREFIX in dac driver"
#endif

#ifndef VSF_DAC_CFG_IMP_UPCASE_PREFIX
#   error "Please define VSF_DAC_CFG_IMP_UPCASE_PREFIX in dac driver"
#endif

#ifndef VSF_DAC_CFG_IMP_INSTANCE_PREFIX
#   define VSF_DAC_CFG_IMP_INSTANCE_PREFIX         VSF_DAC_CFG_IMP_PREFIX
#endif

#include "hal/driver/common/template/vsf_template_implementation.h"

#undef VSF_DAC_CFG_IMP_PREFIX
#undef VSF_DAC_CFG_IMP_INSTANCE_PREFIX
#undef VSF_DAC_CFG_IMP_UPCASE_PREFIX
#undef VSF_DAC_CFG_IMP_LV0

#endif  /* VSF_HAL_USE_DAC */
