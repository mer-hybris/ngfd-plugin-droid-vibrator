#ifndef _NGFD_PLUGIN_VIBRATOR_
#define _NGFD_PLUGIN_VIBRATOR_

#include <ngf/proplist.h>
#include <stdint.h>

#ifdef NATIVE_VIBRATOR

#define IMPLEMENTATION_NAME             "native-vibrator"
#define IMPLEMENTATION_DESCRIPTION      "Haptic feedback using droid vibrator device"
#define NATIVE_FILE_DURATION_PATH_KEY   "native.path"
#define NATIVE_FILE_ACTIVATE_PATH_KEY   "native.activate_path"

#else

#define IMPLEMENTATION_NAME             "droid-vibrator"
#define IMPLEMENTATION_DESCRIPTION      "Haptic feedback using Droid Vibrator HAL via libhybris"

#endif

#define LOG_CAT                     IMPLEMENTATION_NAME ": "

int          h_vibrator_open    (const NProplist *properties);
void         h_vibrator_close   (void);
void         h_vibrator_on      (uint32_t timeout_ms);
void         h_vibrator_off     (void);

#endif
