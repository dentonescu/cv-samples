#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "dmot/log.h"
#include "dmot/timex.h"
#include "wfq/wifiequ.h"

static volatile sig_atomic_t g_running = 1;
static volatile sig_atomic_t g_reload = 0;

typedef struct
{
    int port;
} options;

static void help(FILE *out)
{
    fputs("WiFiEqu help\n", out);
    fputs("============\n", out);
    fputs("--port [port_number]\te.g. 8080", out);
}

static void on_signal(int sig)
{
    if (sig == SIGTERM || sig == SIGINT)
    {
        DMOT_LOGI("Stopping the daemon...\n");
        g_running = 0; // graceful stop
    }
    else if (sig == SIGHUP)
    {
        DMOT_LOGI("Reloading the configuration...\n");
        g_reload = 1; // reload config
    }
}

static void reload_config(void)
{
    // TODO: (re)load configuration here.
    DMOT_LOGD("Reloaded configuration.\n");
    g_reload = 0;
}

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        help(stdout);
        return EXIT_SUCCESS;
    }

    options o;
    bool misunderstood = false;
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--port") && i + 1 < argc)
            o.port = atoi(argv[++i]);
        else
            misunderstood = true;
    }

    if (misunderstood)
    {
        DMOT_LOGE("A parameter was supplied as an argument which was not understood.\n");
        help(stdout);
        return EXIT_FAILURE;
    }

    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);

    // instruct the kernel to use the on_signal handler when the signals listed below arrive
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask); // block during handler
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    DMOT_LOGI("Daemon started with PID %ld\n", (long)getpid());
    DMOT_LOGD("Options: port=%d\n", o.port);

    while (g_running)
    {
        if (g_reload)
            reload_config();

        // TODO: complete me
        sleep_ms(100);
    }

    DMOT_LOGD("Daemon stopped.\n");
    return EXIT_SUCCESS;
}