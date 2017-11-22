#include "implementation.h"

#include <ngf/proplist.h>
#include <ngf/log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define DEFAULT_NATIVE_FILE_PATH "/sys/class/timed_output/vibrator/enable"

static int vibra_fd;

int h_vibrator_open (const NProplist *properties)
{
    const char *device_path;

    vibra_fd = -1;

    if (!(device_path = n_proplist_get_string (properties, NATIVE_FILE_PATH_KEY)))
        device_path = DEFAULT_NATIVE_FILE_PATH;

    if ((vibra_fd = open (device_path, O_WRONLY)) < 0) {
        N_INFO (LOG_CAT "cannot open native vibra control file '%s'", device_path);
        return -1;
    }

    return 0;
}

void h_vibrator_close (void)
{
    if (vibra_fd > 0)
        close (vibra_fd), vibra_fd = -1;
}

static void vibrator_write (uint32_t value)
{
    char value_str[12]; /* fits UINT32_MAX value with newline */
    int length;

    if (vibra_fd < 0)
        return;

    length = snprintf (value_str, sizeof (value_str), "%u\n", value);
    if (write (vibra_fd, value_str, length) != length)
        N_ERROR (LOG_CAT "failed to write to vibra control file");
}

void h_vibrator_on (uint32_t timeout_ms)
{
    vibrator_write (timeout_ms);
}

void h_vibrator_off (void)
{
    vibrator_write (0);
}
