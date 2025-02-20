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
/*============================ INCLUDES ======================================*/
#include "vsf.h"

/*============================ MACROS ========================================*/



/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
declare_vsf_fsm(user_fsm_task_t)
declare_vsf_fsm(user_fsm_sub_task_t)

def_fsm(user_fsm_sub_task_t,
    def_params(
        uint32_t cnt;
    ));


def_fsm(user_fsm_task_t,
    def_params(
        vsf_sem_t *sem_ptr;
        uint32_t cnt;

        vsf_task(user_fsm_sub_task_t) print_task;
    ));

#if VSF_KERNEL_CFG_SUPPORT_THREAD != ENABLED
declare_vsf_fsm(user_task_b_t)
def_fsm(user_task_b_t,
    def_params(
        vsf_sem_t *sem_ptr;
        uint8_t cnt;
    ));
#else
declare_vsf_thread(user_thread_a_t)

def_vsf_thread(user_thread_a_t, 1024,

    features_used(
        mem_sharable( )
        mem_nonsharable( )
    )

    def_params(
        vsf_sem_t *sem_ptr;
    ));
#endif

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static NO_INIT vsf_sem_t __user_sem;
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/


vsf_fsm_initialiser(user_fsm_sub_task_t,
    vsf_args(
        uint8_t chCount
    ))
    vsf_fsm_init_body(
        vsf_this.cnt = chCount;
    )


implement_vsf_fsm(user_fsm_sub_task_t)
    def_states(PRINT_INFO);

    vsf_fsm_body(
        //! this can be ignored
        on_start(
            update_state_to(PRINT_INFO);
        )

        vsf_state(PRINT_INFO) {
            vsf_trace_info("receive semaphore...[%08x]\r\n", vsf_this.cnt++);
            vsf_fsm_cpl();
        }
    )



vsf_fsm_initialiser(user_fsm_task_t,
    vsf_args(
        vsf_sem_t *ptSEM
    ))
    vsf_fsm_init_body(
        vsf_this.cnt = 0;
        vsf_this.sem_ptr = ptSEM;
    )


/*----------------------------------------------------------------------------*
 * Example of unprotected fsm                                                 *
 * NOTE: you can debug content between body_begin() and body_end()            *
 *----------------------------------------------------------------------------*/
implement_vsf_fsm(user_fsm_task_t)
{
    def_states(WAIT_FOR_SEM, CALL_SUB_TO_PRINT);

    vsf_fsm_begin();

    /*! this can be ignored
    on_start(
        update_state_to(WAIT_FOR_SEM);
    )
    */

    vsf_state(WAIT_FOR_SEM) {
        vsf_sem_pend(vsf_this.sem_ptr){                                         //!< wait for semaphore forever
            init_vsf_fsm(   user_fsm_sub_task_t,
                            &vsf_this.print_task,
                            vsf_args(vsf_this.cnt));    //!< init sub fsm
            transfer_to(CALL_SUB_TO_PRINT);                                     //!< transfer to next vsf_state
        }
    }

    vsf_state(CALL_SUB_TO_PRINT) {
        if (fsm_rt_cpl == call_vsf_fsm(user_fsm_sub_task_t, &vsf_this.print_task)) {
            //! fsm complete
            vsf_this.cnt = vsf_this.print_task.cnt;                                     //!< read param value
            reset_vsf_fsm();
        }
    }

    vsf_fsm_end();
}

#if VSF_KERNEL_CFG_SUPPORT_THREAD != ENABLED

/*! \IMPORTANT You cannot ignore vsf_fsm_initialiser at any time
 */
vsf_fsm_initialiser(user_task_b_t,
    args(
        vsf_sem_t *ptSEM
    ))
    vsf_fsm_init_body(
        vsf_this.sem_ptr = ptSEM;
        vsf_this.cnt = 0;
    )

/*----------------------------------------------------------------------------*
 * Example of protected fsm                                                   *
 * NOTE: content in vsf_fsm_body() is protected from debug                            *
 *----------------------------------------------------------------------------*/
implement_vsf_fsm(user_task_b_t)
    def_states(DELAY, PRINT);

    vsf_fsm_body(
        on_start(
            update_state_to(DELAY);
        )

        vsf_state(DELAY){
            vsf_task_wait_until(vsf_delay_ms(3000))                            //!< wait 10s
            update_state_to(PRINT);                                             //!< transfer to PRINT without yielding...
        }

        vsf_state(PRINT){
            vsf_trace_info("post semaphore...   [%08x]\r\n", vsf_this.cnt++);
            vsf_sem_post(vsf_this.sem_ptr);                                            //!< post a semaphore
            reset_vsf_fsm();                                                        //!< reset fsm
        }

    )
#else
implement_vsf_thread(user_thread_a_t)
{
    uint32_t cnt = 0;
    while (1) {
        vsf_delay_ms(3000);
        vsf_trace_info("post semaphore...   [%08x]\r\n", cnt++);
        vsf_sem_post(vsf_this.sem_ptr);            //!< post a semaphore
    }
}

#endif


void vsf_kernel_fsm_simple_demo(void)
{
    //! initialise semaphore
    vsf_sem_init(&__user_sem, 0);

    //! start a user task
    {
        static NO_INIT user_fsm_task_t __user_task;
        init_vsf_fsm(user_fsm_task_t, &(__user_task.param), vsf_args(&__user_sem));
        start_vsf_fsm(user_fsm_task_t, &__user_task, vsf_prio_0);
    };

#if VSF_KERNEL_CFG_SUPPORT_THREAD == ENABLED
    //! start the user task a
    {
        static NO_INIT user_thread_a_t __user_task_a;
        __user_task_a.param.sem_ptr = &__user_sem;
        init_vsf_thread(user_thread_a_t, &__user_task_a, vsf_prio_0);
    }
#else
    //! in this case, we only use main to initialise vsf_tasks
    //! start a user task b
    {
        static NO_INIT user_task_b_t __user_task_b;
        init_vsf_fsm(user_task_b_t, &(__user_task_b.param), args(&__user_sem));
        start_vsf_fsm(user_task_b_t, &__user_task_b, vsf_prio_0);
    };
#endif
}


#if APP_USE_LINUX_DEMO == ENABLED
int kernel_fsm_test_main(int argc, char *argv[])
{
#else
int VSF_USER_ENTRY(void)
{
#   if VSF_USE_TRACE == ENABLED
    vsf_start_trace();
#       if USRAPP_CFG_STDIO_EN == ENABLED
    vsf_stdio_init();
#       endif
#   endif
#endif

    static_task_instance(
        features_used(
            mem_sharable( )
            mem_nonsharable( )
        )
    )

    vsf_kernel_fsm_simple_demo();

#if APP_USE_LINUX_DEMO == ENABLED
    while(1) {
        vsf_trace_info("hello world! \r\n");
        vsf_delay_ms(1000);
    }
#endif
    return 0;
}
