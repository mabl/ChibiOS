#!/usr/bin/env python

import ConfigParser

cfg = ConfigParser.SafeConfigParser()
cfg.read('clock.cfg')

# list of all sections in config
profiles = cfg.sections()

STM32_SYSCLK_MAX        = 72000000
STM32_HSECLK_MAX        = 25000000
STM32_HSECLK_MIN        = 1000000
STM32_LSECLK_MAX        = 1000000
STM32_LSECLK_MIN        = 32768
STM32_PLLIN_MAX         = 25000000
STM32_PLLIN_MIN         = 1000000
STM32_PLLMUL_MAX        = 16
STM32_PLLMUL_MIN        = 2
STM32_PLLOUT_MAX        = 72000000
STM32_PLLOUT_MIN        = 16000000
STM32_PCLK1_MAX         = 36000000
STM32_PCLK2_MAX         = 72000000
STM32_ADCCLK_MAX        = 14000000
STM32_HSE_OSC_MAX       = 16000000
STM32_HSE_OSC_MIN       = 4000000

PLLSRC =        "STM32_PLLSRC_HSE"
PLLXTPRE =      "STM32_PLLXTPRE_DIV1"
PLLMUL_VALUE =  9
HPRE  =         "STM32_HPRE_DIV1"
PPRE1 =         "STM32_PPRE1_DIV2"
PPRE2 =         "STM32_PPRE2_DIV2"
ADCPRE =        "STM32_ADCPRE_DIV4"
USB_CLOCK_REQUIRED = True
USBPRE =        "STM32_USBPRE_DIV1P5"
MCOSEL =        "STM32_MCOSEL_NOCLOCK"
HSECLK =        8000000



def gen_profile(section):
    """
    Generates clock config structure.
    section -- name of section in config, it will be the name of structure
    """
    hsiclk = 8000000
    HSICLK = str(hsiclk)

    sw = cfg.get(section, "SW")#{{{
    if sw == "HSI":
        SW = "STM32_SW_HSI"
        sysclk = HSICLK
    elif sw == "HSE":
        SW = "STM32_SW_HSE"
        hseclk = cfg.getint(section, "HSECLK")
        if hseclk < STM32_HSE_OSC_MIN or hseclk > STM32_HSE_OSC_MAX:
            raise ValueError('HSECKL value out of acceptable bounds')
        else:
            HSECLK = str(hseclk)
            sysclk = HSECLK
    elif sw == "PLL":
        SW = "STM32_SW_PLL"

        pllmul = cfg.getint(section, "PLLMUL")
        if pllmul < STM32_PLLMUL_MIN or pllmul > STM32_PLLMUL_MAX:
            raise ValueError('Acceptable values for PLLMUL is', STM32_PLLMUL_MIN, "..",  STM32_PLLMUL_MAX)
        else:
            PLLMUL = "("+str(pllmul)+" - 2) << 18"

        pllsrc = cfg.get(section, "PLLSRC")
        if pllsrc == "HSE":
            PLLSRC = "STM32_PLLSRC_HSE"

            hseclk = cfg.getint(section, "HSECLK")
            if hseclk < STM32_HSE_OSC_MIN or hseclk > STM32_HSE_OSC_MAX:
                raise ValueError('Acceptable values for HSECKL is from',STM32_HSE_OSC_MIN, "to", STM32_HSE_OSC_MAX)
            else:
                HSECLK = str(hseclk)

            pllxtpre = cfg.getint(section, "PLLXTPRE")
            if pllxtpre == 1:
                PLLXTPRE = "STM32_PLLXTPRE_DIV1"
            elif pllxtpre == 2:
                PLLXTPRE = "STM32_PLLXTPRE_DIV2"
            else:
                raise ValueError('Acceptable values for PLLXTPRE is 1, 2')

            sysclk = pllmul * hseclk / pllxtpre
        elif pllsrc == "HSI":
            PLLSRC = "STM32_PLLSRC_HSE"
            sysclk = pllmul * hsiclk / 2
        else:
            raise ValueError('Acceptable values for PLLSRC is HSI, HSE')

    else:
        raise ValueError('Acceptable values for SW is HSI, HSE, PLL')

    if sysclk > STM32_SYSCLK_MAX:
        raise ValueError('SYSCLK value is', sysclk, 'limit is', STM32_SYSCLK_MAX, "MHz")
    else:
        SYSCLK = str(sysclk)
    #}}}
    hpre = cfg.getint(section, "HPRE")#{{{
    HPRE = ""
    i = 1
    while i <= 512:
        if hpre == i:
            HPRE = "STM32_HPRE_DIV"+str(i)
        i = i * 2
    if HPRE == "":
        raise ValueError("HPRE must be power of 2 and lay between 1..512 inclusively")
    else:
        hclk = sysclk / hpre
        HCLK = str(hclk)
    #}}}
    ppre1 = cfg.getint(section, "PPRE1")#{{{
    PPRE1 = ""
    i = 1
    while i <= 16:
        if ppre1 == i:
            PPRE1 = "STM32_PPRE1_DIV"+str(i)
        i = i * 2
    if PPRE1 == "":
        raise ValueError("PPRE1 must be power of 2 and lay between 1..16 inclusively")

    pclk1 = hclk / ppre1
    PCLK1 = str(pclk1)
    if pclk1 > STM32_PCLK1_MAX:
        raise ValueError("APB1 clock set to", pclk1, "limit is", STM32_PCLK1_MAX)
    #}}}
    ppre2 = cfg.getint(section, "PPRE2")#{{{
    PPRE2 = ""
    i = 1
    while i <= 16:
        if ppre2 == i:
            PPRE2 = "STM32_PPRE1_DIV"+str(i)
        i = i * 2
    if PPRE2 == "":
        raise ValueError("PPRE2 must be power of 2 and lay between 1..16 inclusively")

    pclk2 = hclk / ppre2
    PCLK2 = str(pclk2)
    if pclk2 > STM32_PCLK2_MAX:
        raise ValueError("APB1 clock set to", pclk2, "limit is", STM32_PCLK2_MAX)
    #}}}
    adcpre = cfg.getint(section, "ADCPRE")#{{{
    ADCPRE = ""
    adcdivs = [2, 4, 6, 8]
    for i in adcdivs:
        if adcpre == i:
            ADCPRE = "STM32_ADCPRE_DIV"+str(i)
    if ADCPRE == "":
        raise ValueError("ADCPRE must be one of", adcdivs)

    adcclk = pclk2 / adcpre
    ADCCLK = str(adcclk)
    if adcclk > STM32_ADCCLK_MAX:
        raise ValueError("ADC clock set to", adcclk, "limit is", STM32_ADCCLK_MAX)
    #}}}

    # usbpre = cfg.getfloat(section, "USBPRE")
    # print usbpre



    print(
    "ClockConfig clk_cfg_"+section+" = {\n"
    "   "+SW+",\n"
    "   "+PLLMUL+",\n"
    "   "+HPRE+",\n"
    "   "+PPRE1+",\n"
    "   "+PPRE2+",\n"
    "   "+ADCPRE+",\n"
    "   "+PLLSRC+",\n"
    "   "+PLLXTPRE+",\n"
    "   "+SYSCLK+",\n"
    "   "+HCLK+",\n"
    "   "+PCLK1+",\n"
    "   "+PCLK2+",\n"
    "   "+ADCCLK+",\n"
    "}"
    )



for i in cfg.sections():
    gen_profile(i)




