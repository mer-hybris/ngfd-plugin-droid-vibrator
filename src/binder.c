#include "implementation.h"

#include <ngf/proplist.h>
#include <ngf/log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <gbinder.h>

#define BINDER_SERVICE_DEVICE "/dev/binder"

#define BINDER_SERVICE_IFACE_AIDL "android.hardware.vibrator.IVibrator"
#define BINDER_SERVICE_NAME_AIDL  BINDER_SERVICE_IFACE_AIDL "/default"
#define BINDER_SERVICE_CALLBACK_IFACE_AIDL "android.hardware.vibrator.IVibratorCallback"

#define BINDER_OFF_AIDL 2
#define BINDER_ON_AIDL  3

static GBinderClient         *m_client = NULL;
static gulong                 m_death_id;
static GBinderRemoteObject   *m_remote = NULL;
static GBinderServiceManager *m_service_manager = NULL;
static GBinderLocalObject    *m_callback = NULL;

GBinderLocalReply *vibrator_callback_handler(
    GBinderLocalObject* obj,
    GBinderRemoteRequest* req,
    guint code,
    guint flags,
    int* status,
    void* user_data)
{
    N_DEBUG (LOG_CAT "binder interface callback handler");
}

static void binder_died ();

static int connect()
{
    N_DEBUG (LOG_CAT "connect 1.");
    m_service_manager = gbinder_servicemanager_new (BINDER_SERVICE_DEVICE);

    if (gbinder_servicemanager_wait(m_service_manager, -1)) {
    N_DEBUG (LOG_CAT "connect 2.");
        m_remote = gbinder_servicemanager_get_service_sync (m_service_manager,
             BINDER_SERVICE_NAME_AIDL, NULL);
        if (m_remote) {
    N_DEBUG (LOG_CAT "connect 3.");
            m_death_id = gbinder_remote_object_add_death_handler (
                m_remote, binder_died, NULL);
            m_client = gbinder_client_new (m_remote, BINDER_SERVICE_IFACE_AIDL);
        }
        if (m_client) {
    N_DEBUG (LOG_CAT "connect 4.");
            m_callback = gbinder_servicemanager_new_local_object (
                m_service_manager,
                BINDER_SERVICE_CALLBACK_IFACE_AIDL,
                vibrator_callback_handler,
                NULL);

            N_DEBUG (LOG_CAT "binder interface opened");
            return 0;
        }
    }
    N_DEBUG (LOG_CAT "connect fail.");
    return -1;
}

static void cleanup()
{
    gbinder_remote_object_remove_handler (m_remote, m_death_id);
    m_death_id = 0;
    gbinder_local_object_unref (m_callback);
    m_callback = NULL;

    gbinder_client_unref (m_client);
    m_client = NULL;

    gbinder_servicemanager_unref (m_service_manager);
    m_service_manager = NULL;
    m_remote = NULL; // auto-release
}

void binder_died (GBinderRemoteObject *remote, void *user_data)
{
    N_DEBUG (LOG_CAT "Vibrator service died! Trying to reconnect.");
    cleanup ();
    connect ();
}

int h_vibrator_open (const NProplist *properties)
{
    return connect();
}

void h_vibrator_close (void)
{
    cleanup();
}

static void vibrator_write (int code, uint32_t timeout_ms)
{
    GBinderLocalRequest *req = gbinder_client_new_request2(m_client, code);
    GBinderRemoteReply *reply;
    GBinderReader reader;
    GBinderWriter writer;
    int32_t status;

    gbinder_local_request_init_writer (req, &writer);

    if (code == BINDER_ON_AIDL) {
        gbinder_writer_append_int32 (&writer, timeout_ms);
        gbinder_writer_append_local_object (&writer, m_callback);
    }
    reply = gbinder_client_transact_sync_reply (m_client, code, req, &status);
    gbinder_local_request_unref (req);

    if (status != GBINDER_STATUS_OK) {
        N_ERROR (LOG_CAT "failed to write to binder (status %d)", status);
    }
}

void h_vibrator_on (uint32_t timeout_ms)
{
    vibrator_write (BINDER_ON_AIDL, timeout_ms);
}

void h_vibrator_off (void)
{
    vibrator_write (BINDER_OFF_AIDL, 0);
}
