#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

/* Use _WIN32 to automaticlly detect real system OS */
#ifndef _WIN32

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/Xrandr.h>

#ifdef FGLRX

#include <fglrx_gamma.h>

#endif

#else

#include <windows.h>
#include <wingdi.h>

#endif

#include <math.h>

/* the 4-byte marker for the vcgt-Tag */
#define VCGT_TAG 0x76636774L
#define MLUT_TAG 0x6d4c5554L

#ifndef DPC_VERSION
#define DPC_VERSION "version unknown"
#endif

/* a limit to check the table sizes (of corrupted profiles) */
#ifndef MAX_TABLE_SIZE
#define MAX_TABLE_SIZE 2e10
#endif

#ifdef _WIN32
#define u_int16_t WORD
#endif

void error(char *fmt, ...), warning(char *fmt, ...), message(char *fmt, ...);

#if 1
#define BE_INT(a) ((a)[3] + ((a)[2] << 8) + ((a)[1] << 16) + ((a)[0] << 24))
#define BE_SHORT(a) ((a)[1] + ((a)[0] << 8))
#define ROUND(a) ((a) + 0.5)
#else
#warning "big endian is NOT TESTED"
#define BE_INT(a) (a)
#define BE_SHORT(a) (a)
#endif

/* internal state struct */
struct dpc_state_t
{
    unsigned int verbose;
    float redGamma;
    float redMin;
    float redMax;
    float greenGamma;
    float greenMin;
    float greenMax;
    float blueGamma;
    float blueMin;
    float blueMax;
    float gamma_cor;
} dpc_state = {0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0};

void usage(void)
{
    fprintf(stdout, "Display Profile Calibrator %s\n", DPC_VERSION);
    fprintf(stdout, "sfc99892\n");
    fprintf(stdout, "USE THIS PROGRAM AT YOUR OWN RISK!\n");
    fprintf(stdout, "IT MAY DAMAGE YOUR COMPUTER!!!\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Usage:  dpc [-options] ICCPROFILE\n");
    fprintf(stdout, "     or dpc [-options] -alter\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Other Commands:\n");
#ifndef _WIN32
    fprintf(stdout, "    -display <host:dpy>     or -d\n");
    fprintf(stdout, "    -screen <screen-#>      or -s\n");
    fprintf(stdout, "    -output <output-#>      or -o\n");
#else
    fprintf(stdout, "    -screen <monitor-#>     or -s\n");
#endif
#ifdef FGLRX
    fprintf(stdout, "    -controller <card-#>    or -x\n");
#endif
    fprintf(stdout, "    -clear                  or -c\n");
    fprintf(stdout, "    -noaction <LUT-size>    or -n\n");
    fprintf(stdout, "    -verbose                or -v\n");
    fprintf(stdout, "    -printramps             or -p\n");
    fprintf(stdout, "    -loss                   or -l\n");
    fprintf(stdout, "    -invert                 or -i\n");
    fprintf(stdout, "    -gammacor <gamma>       or -gc\n");
    fprintf(stdout, "    -brightness <percent>   or -b\n");
    fprintf(stdout, "    -contrast <percent>     or -co\n");
    fprintf(stdout, "    -red <gamma>   <brightness-percentage> <contrast-percentage>\n");
    fprintf(stdout, "    -green <gamma> <brightness-percentage> <contrast-percentage>\n");
    fprintf(stdout, "    -blue <gamma>  <brightness-percentage> <contrast-percentage>\n");
#ifndef FGLRX
    fprintf(stdout, "    -alter                  or -a\n");
#endif
    fprintf(stdout, "    -help                   or -h\n");
    fprintf(stdout, "    -version\n");
    fprintf(stdout, "\n");
    fprintf(stdout,
            "last parameter must be an ICC profile containing a vcgt-tag\n");
    fprintf(stdout, "\n");
#ifndef _WIN32
    fprintf(stdout, "Example: ./dpc -d :0 -s 0 -v bluish.icc\n");
#else
    fprintf(stdout, "Example: ./dpc -v bluish.icc\n");
#endif
#ifndef FGLRX
    fprintf(stdout, "Example: ./dpc -red 1.1 10.0 100.0\n");
#endif
    fprintf(stdout, "*** If you mess up your screen display, use command below to cancel it: ***\n");
    fprintf(stdout, "*** ./dpc -clear\n");
    fprintf(stdout, "\n");
    exit(0);
}

