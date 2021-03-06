/*
 * Copyright (c) 2012, Chris Andrews. All rights reserved.
 */

#include <stdint.h>
#include <unistd.h>

typedef uint8_t usdt_argtype_t;
#define USDT_ARGTYPE_NONE    0
#define USDT_ARGTYPE_STRING  1
#define USDT_ARGTYPE_INTEGER 2

#define USDT_ARG_MAX 32

typedef enum usdt_error {
        USDT_ERROR_MALLOC = 0,
        USDT_ERROR_VALLOC,
        USDT_ERROR_NOPROBES,
        USDT_ERROR_LOADDOF,
        USDT_ERROR_ALREADYENABLED
} usdt_error_t;

typedef struct usdt_probe {
        int (*isenabled_addr)(void);
        void *probe_addr;
} usdt_probe_t;

int usdt_is_enabled(usdt_probe_t *probe);
void usdt_fire_probe(usdt_probe_t *probe, size_t argc, void **argv);

typedef struct usdt_probedef {
        const char *name;
        const char *function;
        size_t argc;
        usdt_argtype_t types[USDT_ARG_MAX];
        struct usdt_probe *probe;
        struct usdt_probedef *next;
} usdt_probedef_t;

usdt_probedef_t *usdt_create_probe(const char *func, const char *name,
                                   size_t argc, const char **types);

typedef struct usdt_provider {
        const char *name;
        const char *module;
        usdt_probedef_t *probedefs;
        char *error;
        int enabled;
} usdt_provider_t;

usdt_provider_t *usdt_create_provider(const char *name, const char *module);
void usdt_provider_add_probe(usdt_provider_t *provider, usdt_probedef_t *probedef);
int usdt_provider_enable(usdt_provider_t *provider);

void usdt_error(usdt_provider_t *provider, usdt_error_t error, ...);
char *usdt_errstr(usdt_provider_t *provider);
