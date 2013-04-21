[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="mcuconf.h" /]
/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * SPC56ELxx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 1...15       Lowest...Highest.
 */

#define SPC56ELxx_MCUCONF

/*
 * HAL driver system settings.
 */
#define SPC5_NO_INIT                        ${conf.instance.initialization_settings.do_not_init[0]?upper_case}
#define SPC5_ALLOW_OVERCLOCK                ${conf.instance.initialization_settings.allow_overclocking[0]?upper_case}
#define SPC5_DISABLE_WATCHDOG               ${conf.instance.initialization_settings.disable_watchdog[0]?upper_case}
#define SPC5_FMPLL0_CLK_SRC                 SPC5_FMPLL_SRC_${conf.instance.initialization_settings.fmpll0_settings.clock_source[0]}
#define SPC5_FMPLL0_IDF_VALUE               ${conf.instance.initialization_settings.fmpll0_settings.idf_value[0]}
#define SPC5_FMPLL0_NDIV_VALUE              ${conf.instance.initialization_settings.fmpll0_settings.ndiv_value[0]}
#define SPC5_FMPLL0_ODF                     ${conf.instance.initialization_settings.fmpll0_settings.odf_value[0]}
#define SPC5_FMPLL1_CLK_SRC                 SPC5_FMPLL_SRC_${conf.instance.initialization_settings.fmpll1_settings.clock_source[0]}
#define SPC5_FMPLL1_IDF_VALUE               ${conf.instance.initialization_settings.fmpll1_settings.idf_value[0]}
#define SPC5_FMPLL1_NDIV_VALUE              ${conf.instance.initialization_settings.fmpll1_settings.ndiv_value[0]}
#define SPC5_FMPLL1_ODF                     ${conf.instance.initialization_settings.fmpll1_settings.odf_value[0]}
#define SPC5_SYSCLK_DIVIDER_VALUE           ${conf.instance.initialization_settings.clocks.system_clock_divider[0]}
#define SPC5_AUX0CLK_SRC                    SPC5_CGM_SS_${conf.instance.initialization_settings.clocks.aux0_clock_source[0]}
#define SPC5_MCONTROL_DIVIDER_VALUE         ${conf.instance.initialization_settings.clocks.motor_control_clock_divider[0]}
#define SPC5_SWG_DIVIDER_VALUE              ${conf.instance.initialization_settings.clocks.swg_clock_divider[0]}
#define SPC5_AUX1CLK_SRC                    SPC5_CGM_SS_${conf.instance.initialization_settings.clocks.aux1_clock_source[0]}
#define SPC5_FLEXRAY_DIVIDER_VALUE          ${conf.instance.initialization_settings.clocks.flexray_clock_divider[0]}
#define SPC5_AUX2CLK_SRC                    SPC5_CGM_SS_${conf.instance.initialization_settings.clocks.aux2_clock_source[0]}
#define SPC5_FLEXCAN_DIVIDER_VALUE          ${conf.instance.initialization_settings.clocks.flexcan_clock_divider[0]}
#define SPC5_ME_ME_BITS                     (SPC5_ME_ME_RUN1 |              \
                                             SPC5_ME_ME_RUN2 |              \
                                             SPC5_ME_ME_RUN3 |              \
                                             SPC5_ME_ME_HALT0 |             \
                                             SPC5_ME_ME_STOP0)
#define SPC5_ME_SAFE_MC_BITS                (SPC5_ME_MC_PDO)
#define SPC5_ME_DRUN_MC_BITS                (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_RUN0_MC_BITS                (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_RUN1_MC_BITS                (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_RUN2_MC_BITS                (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_RUN3_MC_BITS                (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_HALT0_MC_BITS               (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_STOP0_MC_BITS               (SPC5_ME_MC_SYSCLK_FMPLL0 |     \
                                             SPC5_ME_MC_IRCON |             \
                                             SPC5_ME_MC_XOSC0ON |           \
                                             SPC5_ME_MC_PLL0ON |            \
                                             SPC5_ME_MC_PLL1ON |            \
                                             SPC5_ME_MC_FLAON_NORMAL |      \
                                             SPC5_ME_MC_MVRON)
#define SPC5_ME_RUN_PC3_BITS                (SPC5_ME_RUN_PC_RUN0 |          \
                                             SPC5_ME_RUN_PC_RUN1 |          \
                                             SPC5_ME_RUN_PC_RUN2 |          \
                                             SPC5_ME_RUN_PC_RUN3)
#define SPC5_ME_RUN_PC4_BITS                (SPC5_ME_RUN_PC_RUN0 |          \
                                             SPC5_ME_RUN_PC_RUN1 |          \
                                             SPC5_ME_RUN_PC_RUN2 |          \
                                             SPC5_ME_RUN_PC_RUN3)
#define SPC5_ME_RUN_PC5_BITS                (SPC5_ME_RUN_PC_RUN0 |          \
                                             SPC5_ME_RUN_PC_RUN1 |          \
                                             SPC5_ME_RUN_PC_RUN2 |          \
                                             SPC5_ME_RUN_PC_RUN3)
#define SPC5_ME_RUN_PC6_BITS                (SPC5_ME_RUN_PC_RUN0 |          \
                                             SPC5_ME_RUN_PC_RUN1 |          \
                                             SPC5_ME_RUN_PC_RUN2 |          \
                                             SPC5_ME_RUN_PC_RUN3)
#define SPC5_ME_RUN_PC7_BITS                (SPC5_ME_RUN_PC_RUN0 |          \
                                             SPC5_ME_RUN_PC_RUN1 |          \
                                             SPC5_ME_RUN_PC_RUN2 |          \
                                             SPC5_ME_RUN_PC_RUN3)
#define SPC5_ME_LP_PC4_BITS                 (SPC5_ME_LP_PC_HALT0 |          \
                                             SPC5_ME_LP_PC_STOP0)
#define SPC5_ME_LP_PC5_BITS                 (SPC5_ME_LP_PC_HALT0 |          \
                                             SPC5_ME_LP_PC_STOP0)
#define SPC5_ME_LP_PC6_BITS                 (SPC5_ME_LP_PC_HALT0 |          \
                                             SPC5_ME_LP_PC_STOP0)
#define SPC5_ME_LP_PC7_BITS                 (SPC5_ME_LP_PC_HALT0 |          \
                                             SPC5_ME_LP_PC_STOP0)
#define SPC5_CLOCK_FAILURE_HOOK()           ${conf.instance.initialization_settings.clocks.clock_failure_hook[0]}

/*
 * PWM driver system settings.
 */
#define SPC5_PWM_USE_SMOD0                  ${conf.instance.flexpwm_settings.flexpwm0_sm0[0]?upper_case}
#define SPC5_PWM_USE_SMOD1                  ${conf.instance.flexpwm_settings.flexpwm0_sm1[0]?upper_case}
#define SPC5_PWM_USE_SMOD2                  ${conf.instance.flexpwm_settings.flexpwm0_sm2[0]?upper_case}
#define SPC5_PWM_USE_SMOD3                  ${conf.instance.flexpwm_settings.flexpwm0_sm3[0]?upper_case}
#define SPC5_PWM_SMOD0_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm0_sm0[0]}
#define SPC5_PWM_SMOD1_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm0_sm1[0]}
#define SPC5_PWM_SMOD2_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm0_sm2[0]}
#define SPC5_PWM_SMOD3_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm0_sm3[0]}
#define SPC5_PWM_FLEXPWM0_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_PWM_FLEXPWM0_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#define SPC5_PWM_USE_SMOD4                  ${conf.instance.flexpwm_settings.flexpwm1_sm0[0]?upper_case}
#define SPC5_PWM_USE_SMOD5                  ${conf.instance.flexpwm_settings.flexpwm1_sm1[0]?upper_case}
#define SPC5_PWM_USE_SMOD6                  ${conf.instance.flexpwm_settings.flexpwm1_sm2[0]?upper_case}
#define SPC5_PWM_USE_SMOD7                  ${conf.instance.flexpwm_settings.flexpwm1_sm3[0]?upper_case}
#define SPC5_PWM_SMOD4_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm1_sm0[0]}
#define SPC5_PWM_SMOD5_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm1_sm1[0]}
#define SPC5_PWM_SMOD6_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm1_sm2[0]}
#define SPC5_PWM_SMOD7_PRIORITY             ${conf.instance.irq_priority_settings.flexpwm1_sm3[0]}
#define SPC5_PWM_FLEXPWM1_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_PWM_FLEXPWM1_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

/*
 * ICU driver system settings.
 */
#define SPC5_ICU_USE_SMOD0                  ${conf.instance.etimer_settings.etimer0_ch0[0]?upper_case}
#define SPC5_ICU_USE_SMOD1                  ${conf.instance.etimer_settings.etimer0_ch1[0]?upper_case}
#define SPC5_ICU_USE_SMOD2                  ${conf.instance.etimer_settings.etimer0_ch2[0]?upper_case}
#define SPC5_ICU_USE_SMOD3                  ${conf.instance.etimer_settings.etimer0_ch3[0]?upper_case}
#define SPC5_ICU_USE_SMOD4                  ${conf.instance.etimer_settings.etimer0_ch4[0]?upper_case}
#define SPC5_ICU_USE_SMOD5                  ${conf.instance.etimer_settings.etimer0_ch5[0]?upper_case}
#define SPC5_ICU_ETIMER0_PRIORITY           ${conf.instance.irq_priority_settings.etimer0[0]}
#define SPC5_ICU_ETIMER0_START_PCTL         (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_ICU_ETIMER0_STOP_PCTL          (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#define SPC5_ICU_USE_SMOD6                  ${conf.instance.etimer_settings.etimer1_ch0[0]?upper_case}
#define SPC5_ICU_USE_SMOD7                  ${conf.instance.etimer_settings.etimer1_ch1[0]?upper_case}
#define SPC5_ICU_USE_SMOD8                  ${conf.instance.etimer_settings.etimer1_ch2[0]?upper_case}
#define SPC5_ICU_USE_SMOD9                  ${conf.instance.etimer_settings.etimer1_ch3[0]?upper_case}
#define SPC5_ICU_USE_SMOD10                 ${conf.instance.etimer_settings.etimer1_ch4[0]?upper_case}
#define SPC5_ICU_USE_SMOD11                 ${conf.instance.etimer_settings.etimer1_ch5[0]?upper_case}
#define SPC5_ICU_ETIMER1_PRIORITY           ${conf.instance.irq_priority_settings.etimer1[0]}
#define SPC5_ICU_ETIMER1_START_PCTL         (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_ICU_ETIMER1_STOP_PCTL          (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#define SPC5_ICU_USE_SMOD12                 ${conf.instance.etimer_settings.etimer2_ch0[0]?upper_case}
#define SPC5_ICU_USE_SMOD13                 ${conf.instance.etimer_settings.etimer2_ch1[0]?upper_case}
#define SPC5_ICU_USE_SMOD14                 ${conf.instance.etimer_settings.etimer2_ch2[0]?upper_case}
#define SPC5_ICU_USE_SMOD15                 ${conf.instance.etimer_settings.etimer2_ch3[0]?upper_case}
#define SPC5_ICU_USE_SMOD16                 ${conf.instance.etimer_settings.etimer2_ch4[0]?upper_case}
#define SPC5_ICU_USE_SMOD17                 ${conf.instance.etimer_settings.etimer2_ch5[0]?upper_case}
#define SPC5_ICU_ETIMER2_PRIORITY           ${conf.instance.irq_priority_settings.etimer2[0]}
#define SPC5_ICU_ETIMER2_START_PCTL         (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_ICU_ETIMER2_STOP_PCTL          (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

/*
 * SERIAL driver system settings.
 */
#define SPC5_SERIAL_USE_LINFLEX0            ${(conf.instance.linflex_settings.linflex0[0] == "Serial")?string?upper_case}
#define SPC5_SERIAL_USE_LINFLEX1            ${(conf.instance.linflex_settings.linflex1[0] == "Serial")?string?upper_case}
#define SPC5_SERIAL_LINFLEX0_PRIORITY       ${conf.instance.irq_priority_settings.linflex0[0]}
#define SPC5_SERIAL_LINFLEX1_PRIORITY       ${conf.instance.irq_priority_settings.linflex1[0]}
#define SPC5_SERIAL_LINFLEX0_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_SERIAL_LINFLEX0_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#define SPC5_SERIAL_LINFLEX1_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_SERIAL_LINFLEX1_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
