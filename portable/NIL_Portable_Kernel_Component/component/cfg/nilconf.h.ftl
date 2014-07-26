[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="nilconf.h" /]
/*
    ChibiOS/NIL - Copyright (C) 2013,2014 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    nilconf.h
 * @brief   Configuration file template.
 * @details A copy of this file must be placed in each project directory, it
 *          contains the application specific kernel settings.
 *
 * @addtogroup config
 * @details Kernel related settings and hooks.
 * @{
 */

#ifndef _NILCONF_H_
#define _NILCONF_H_

/*===========================================================================*/
/**
 * @name Kernel parameters and options
 * @{
 */
/*===========================================================================*/

/**
 * @brief   Number of user threads in the application.
 * @note    This number is not inclusive of the idle thread which is
 *          Implicitly handled.
 */
#define NIL_CFG_NUM_THREADS                 ${conf.instance.tasks_configuration.tasks.*?size}

/** @} */

/*===========================================================================*/
/**
 * @name System timer settings
 * @{
 */
/*===========================================================================*/

/**
 * @brief   System time counter resolution.
 * @note    Allowed values are 16 or 32 bits.
 */
#define NIL_CFG_ST_RESOLUTION               ${conf.instance.general_options.system_tick_counter_resolution.value[0]}

/**
 * @brief   System tick frequency.
 * @note    This value together with the @p NIL_CFG_ST_RESOLUTION
 *          option defines the maximum amount of time allowed for
 *          timeouts.
 */
#define NIL_CFG_ST_FREQUENCY                ${conf.instance.general_options.system_tick_clock_frequency.value[0]}

/**
 * @brief   Time delta constant for the tick-less mode.
 * @note    If this value is zero then the system uses the classic
 *          periodic tick. This value represents the minimum number
 *          of ticks that is safe to specify in a timeout directive.
 *          The value one is not valid, timeouts are rounded up to
 *          this value.
 */
[#if conf.instance.general_options.system_tick_mode.@index[0]?number == 0]
#define NIL_CFG_ST_TIMEDELTA                0
[#else]
#define NIL_CFG_ST_TIMEDELTA                ${conf.instance.general_options.system_tick_delta.value[0]}
[/#if]

/** @} */

/*===========================================================================*/
/**
 * @name Subsystem options
 * @{
 */
/*===========================================================================*/

/**
 * @brief   Events Flags APIs.
 * @details If enabled then the event flags APIs are included in the kernel.
 *
 * @note    The default is @p TRUE.
 */
#define NIL_CFG_USE_EVENTS                  ${conf.instance.subsystem_options.use_events.value[0]?upper_case}

/** @} */

/*===========================================================================*/
/**
 * @name Debug options
 * @{
 */
/*===========================================================================*/

/**
 * @brief   System assertions.
 */
#define NIL_CFG_ENABLE_ASSERTS              ${conf.instance.debug_options.enable_assertions.value[0]?upper_case}

/**
 * @brief   Stack check.
 */
#define NIL_CFG_ENABLE_STACK_CHECK          ${conf.instance.debug_options.enable_stack_check.value[0]?upper_case}

/** @} */

/*===========================================================================*/
/**
 * @name Kernel hooks
 * @{
 */
/*===========================================================================*/

/**
 * @brief   System initialization hook.
 */
#if !defined(NIL_CFG_SYSTEM_INIT_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_SYSTEM_INIT_HOOK() {                                        \
${conf.instance.hooks.system_initialization_hook.value[0]}[#rt/]
}
#endif

/**
 * @brief   Threads descriptor structure extension.
 * @details User fields added to the end of the @p thread_t structure.
 */
[#if conf.instance.hooks.thread_extra_fields_hook.value[0]?trim == ""]
#define NIL_CFG_THREAD_EXT_FIELDS
[#else]
#define NIL_CFG_THREAD_EXT_FIELDS                                           \
${conf.instance.hooks.thread_extra_fields_hook.value[0]}
[/#if]

/**
 * @brief   Threads initialization hook.
 */
#define NIL_CFG_THREAD_EXT_INIT_HOOK(tr) {                                  \
${conf.instance.hooks.thread_initialization_hook.value[0]}[#rt/]
}

/**
 * @brief   Idle thread enter hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to activate a power saving mode.
 */
#define NIL_CFG_IDLE_ENTER_HOOK() {                                         \
${conf.instance.hooks.idle_enter_hook.value[0]}[#rt/]
}

/**
 * @brief   Idle thread leave hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to deactivate a power saving mode.
 */
#define NIL_CFG_IDLE_LEAVE_HOOK() {                                         \
${conf.instance.hooks.idle_leave_hook.value[0]}[#rt/]
}

/**
 * @brief   System halt hook.
 */
#if !defined(NIL_CFG_SYSTEM_HALT_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_SYSTEM_HALT_HOOK(reason) {                                  \
${conf.instance.hooks.system_halt_hook.value[0]}[#rt/]
}
#endif

/** @} */

/*===========================================================================*/
/* Port-specific settings (override port settings defaulted in nilcore.h).   */
/*===========================================================================*/

#endif  /* _NILCONF_H_ */

/** @} */
