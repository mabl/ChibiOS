/*
 MB963xx series CPU definition
*/

#ifndef	_f16fxs_h
#define	_f16fxs_h

#if defined(__CPU_MB96F313Yxx__) || \
    defined(__CPU_MB96F313Rxx__) || \
    defined(__CPU_MB96F313Axx__) || \
    defined(__CPU_MB96F315Yxx__) || \
    defined(__CPU_MB96F315Rxx__) || \
    defined(__CPU_MB96F315Axx__)
#error MB96310 not supported yet

#include "mb96310.h"

#define	__CPU_MB96310_SERIES

#elif defined(__CPU_MB96F326ASA__) || \
    defined(__CPU_MB96F326AWA__) || \
    defined(__CPU_MB96F326YSA__) || \
    defined(__CPU_MB96F326RSA__) || \
    defined(__CPU_MB96F326YWA__) || \
    defined(__CPU_MB96F326RWA__)
#error MB96320 not supported yet

#include "mb96320.h"

#define	__CPU_MB96320_SERIES

#elif defined(__CPU_MB96F338YSA__) || \
    defined(__CPU_MB96F338YWA__) || \
    defined(__CPU_MB96F338USA__) || \
    defined(__CPU_MB96F338UWA__) || \
    defined(__CPU_MB96F338RSA__) || \
    defined(__CPU_MB96F338RWA__)

#include "mb96338us.h"	// specific cpu type

#define	__CPU_MB96330_SERIES

#elif defined(__CPU_MB96F346ASA__) || \
    defined(__CPU_MB96F346AWA__) || \
    defined(__CPU_MB96F346YSA__) || \
    defined(__CPU_MB96F346RSA__) || \
    defined(__CPU_MB96F346YWA__) || \
    defined(__CPU_MB96F346RWA__) || \
    defined(__CPU_MB96F347ASA__) || \
    defined(__CPU_MB96F347AWA__) || \
    defined(__CPU_MB96F347YSA__) || \
    defined(__CPU_MB96F347RSA__) || \
    defined(__CPU_MB96F347YWA__) || \
    defined(__CPU_MB96F347RWA__) || \
    defined(__CPU_MB96F348ASA__) || \
    defined(__CPU_MB96F348AWA__) || \
    defined(__CPU_MB96F348CSA__) || \
    defined(__CPU_MB96F348CWA__) || \
    defined(__CPU_MB96F348TSA__) || \
    defined(__CPU_MB96F348TSB__) || \
    defined(__CPU_MB96F348HSA__) || \
    defined(__CPU_MB96F348HSB__) || \
    defined(__CPU_MB96F348TWA__) || \
    defined(__CPU_MB96F348TWB__) || \
    defined(__CPU_MB96F348HWA__) || \
    defined(__CPU_MB96F348HWB__) || \
    defined(__CPU_MB96F348YSA__) || \
    defined(__CPU_MB96F348RSA__) || \
    defined(__CPU_MB96F348YWA__) || \
    defined(__CPU_MB96F348RWA__) || \
    defined(__CPU_MB96F346YSB__) || \
    defined(__CPU_MB96F346RSB__) || \
    defined(__CPU_MB96F346YWB__) || \
    defined(__CPU_MB96F346RWB__) || \
    defined(__CPU_MB96F347YSB__) || \
    defined(__CPU_MB96F347RSB__) || \
    defined(__CPU_MB96F347YWB__) || \
    defined(__CPU_MB96F347RWB__) || \
    defined(__CPU_MB96F348YSB__) || \
    defined(__CPU_MB96F348RSB__) || \
    defined(__CPU_MB96F348YWB__) || \
    defined(__CPU_MB96F348RWB__) || \
    defined(__CPU_MB96F348TSC__) || \
    defined(__CPU_MB96F348HSC__) || \
    defined(__CPU_MB96F348TWC__) || \
    defined(__CPU_MB96F348HWC__) || \
    defined(__CPU_MB96345YSA__) || \
    defined(__CPU_MB96345RSA__) || \
    defined(__CPU_MB96345YWA__) || \
    defined(__CPU_MB96345RWA__) || \
    defined(__CPU_MB96346YSA__) || \
    defined(__CPU_MB96346RSA__) || \
    defined(__CPU_MB96346YWA__) || \
    defined(__CPU_MB96346RWA__) || \
    defined(__CPU_MB96F346ASB__) || \
    defined(__CPU_MB96F346AWB__) || \
    defined(__CPU_MB96F347ASB__) || \
    defined(__CPU_MB96F347AWB__) || \
    defined(__CPU_MB96F348ASB__) || \
    defined(__CPU_MB96F348AWB__)

#include "mb96348hs.h"	// specific cpu type

#define	__CPU_MB96340_SERIES


#elif defined(__CPU_MB96F356ASA__) || \
    defined(__CPU_MB96F356AWA__) || \
    defined(__CPU_MB96F356RSA__) || \
    defined(__CPU_MB96F356RWA__) || \
    defined(__CPU_MB96F356RWB__) || \
    defined(__CPU_MB96F356YSA__) || \
    defined(__CPU_MB96F356YWA__)

#include "mb96356rw.h"	// specific cpu type

#define	__CPU_MB96350_SERIES


#elif defined(__CPU_MB96F386YSA__) || \
    defined(__CPU_MB96F386RSA__) || \
    defined(__CPU_MB96F386YWA__) || \
    defined(__CPU_MB96F386RWA__) || \
    defined(__CPU_MB96F387YSA__) || \
    defined(__CPU_MB96F387RSA__) || \
    defined(__CPU_MB96F387YWA__) || \
    defined(__CPU_MB96F387RWA__) || \
    defined(__CPU_MB96384YSA__) || \
    defined(__CPU_MB96384RSA__) || \
    defined(__CPU_MB96384YWA__) || \
    defined(__CPU_MB96384RWA__) || \
    defined(__CPU_MB96385YSA__) || \
    defined(__CPU_MB96385RSA__) || \
    defined(__CPU_MB96385YWA__) || \
    defined(__CPU_MB96385RWA__) || \
    defined(__CPU_MB96F388TSA__) || \
    defined(__CPU_MB96F388HSA__) || \
    defined(__CPU_MB96F388TWA__) || \
    defined(__CPU_MB96F388HWA__) || \
    defined(__CPU_MB96F389YSA__) || \
    defined(__CPU_MB96F389RSA__) || \
    defined(__CPU_MB96F389YWA__) || \
    defined(__CPU_MB96F389RWA__) || \
    defined(__CPU_MB96F386YSB__) || \
    defined(__CPU_MB96F386RSB__) || \
    defined(__CPU_MB96F386YWB__) || \
    defined(__CPU_MB96F386RWB__) || \
    defined(__CPU_MB96F387YSB__) || \
    defined(__CPU_MB96F387RSB__) || \
    defined(__CPU_MB96F387YWB__) || \
    defined(__CPU_MB96F387RWB__)
#error MB96310 not supported yet

#include "mb96380.h"

#define	__CPU_MB96380_SERIES


#else
	#define __NOT_MB963xx_SERIES
	#error "This ChibiOS/RT is for MB963xx only. Please check project settings"
#endif

#define	CPU_CLOCK	48000000L

#endif	// _f16fxs_h
