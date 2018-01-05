#include "implementation.h"

#include <ngf/proplist.h>
#include <ngf/log.h>

#include <android-version.h>
#if ANDROID_VERSION_MAJOR >= 7
#include <hardware/vibrator.h>
#else
#include <hardware_legacy/vibrator.h>
#endif

#if ANDROID_VERSION_MAJOR >= 7
static vibrator_device_t *dev;
#endif

int h_vibrator_open (const NProplist *properties)
{
    (void) properties;

#if ANDROID_VERSION_MAJOR >= 7
    struct hw_module_t *hwmod;

    hw_get_module (VIBRATOR_HARDWARE_MODULE_ID, (const hw_module_t **)(&hwmod));
    g_assert(hwmod != NULL);
    dev = NULL;

    if (vibrator_open (hwmod, &dev) < 0) {
        N_DEBUG (LOG_CAT "unable to open vibrator device");
        return -1;
    }
#endif
    return 0;
}

void h_vibrator_close (void)
{
#if ANDROID_VERSION_MAJOR >= 7
    if (dev)
        dev->common.close((hw_device_t *) dev), dev = NULL;
#endif
}

void h_vibrator_on (uint32_t timeout_ms)
{
#if ANDROID_VERSION_MAJOR >= 7
    if (dev)
        dev->vibrator_on (dev, timeout_ms);
#else
    vibrator_on (timeout_ms);
#endif

}

void h_vibrator_off (void)
{
#if ANDROID_VERSION_MAJOR >= 7
    dev->vibrator_off (dev);
#else
    vibrator_off ();
#endif
}
