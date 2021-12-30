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
