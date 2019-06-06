#include "implementation.h"

#include <ngf/proplist.h>
#include <ngf/log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

struct file_location {
    const char *duration;
    const char *activate;
};

static struct file_location file_locations[] = {
    { NULL,                                         NULL }, /* Filled from user set values */
    { "/sys/class/timed_output/vibrator/enable",    NULL },
    { "/sys/class/leds/vibrator/duration",          "/sys/class/leds/vibrator/activate" },
};

#define ACTIVATE_ON     (1)
#define ACTIVATE_OFF    (0)

static int duration_fd;
static int activate_fd;

int h_vibrator_open (const NProplist *properties)
{
    const char *duration_path;
    const char *activate_path;
    int count = sizeof(file_locations) / sizeof(file_locations[0]);
    int i = 0;

    duration_fd = -1;
    activate_fd = -1;

    if ((file_locations[0].duration = n_proplist_get_string (properties, NATIVE_FILE_DURATION_PATH_KEY)))
        file_locations[0].activate = n_proplist_get_string (properties, NATIVE_FILE_ACTIVATE_PATH_KEY);
    else
        i++;

    for (; i < count; i++) {
        duration_path = file_locations[i].duration;
        activate_path = file_locations[i].activate;
        N_DEBUG (LOG_CAT "look for %s %s", duration_path, activate_path ? activate_path : "<none>");

        if ((duration_fd = open (duration_path, O_WRONLY)) < 0) {
            duration_path = NULL;
            continue;
        }

        if (activate_path) {
            if ((activate_fd = open (activate_path, O_WRONLY)) < 0) {
                h_vibrator_close ();
                duration_path = NULL;
                activate_path = NULL;
                continue;
            }
        }

        break;
    }

    if (duration_fd < 0) {
        N_INFO (LOG_CAT "could not open native vibra control.");
        return -1;
    }

    N_DEBUG (LOG_CAT "open native vibrator control path: %s%s%s",
                     duration_path,
                     activate_fd >= 0 ? " activate path: " : "",
                     activate_fd >= 0 ? activate_path : "");

    return 0;
}

void h_vibrator_close (void)
{
    if (duration_fd >= 0)
        close (duration_fd), duration_fd = -1;
    if (activate_fd >= 0)
        close (activate_fd), activate_fd = -1;
}

static void vibrator_write (int fd, uint32_t value)
{
    char value_str[12]; /* fits UINT32_MAX value with newline */
    int length;

    if (fd < 0)
        return;

    length = snprintf (value_str, sizeof (value_str), "%u\n", value);
    if (write (fd, value_str, length) != length)
        N_ERROR (LOG_CAT "failed to write to control file (fd %d)", fd);
}

void h_vibrator_on (uint32_t timeout_ms)
{
    vibrator_write (duration_fd, timeout_ms);
    if (activate_fd >= 0)
        vibrator_write (activate_fd, ACTIVATE_ON);
}

void h_vibrator_off (void)
{
    if (activate_fd >= 0)
        vibrator_write (activate_fd, ACTIVATE_OFF);
    else
        vibrator_write (duration_fd, ACTIVATE_OFF);
}
