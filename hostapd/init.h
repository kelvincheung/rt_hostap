/*
 * hostapd - Internal definitions
 * Copyright (c) 2003-2010, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef INIT_H
#define INIT_H

#include "ap/hostapd.h"

int hostapd_driver_init(struct hostapd_iface *iface);
int hostapd_global_init(struct hapd_interfaces *interfaces);
struct hostapd_iface *
hostapd_interface_init(struct hapd_interfaces *interfaces,
		       const char *config_fname, int debug);
int hostapd_global_run(struct hapd_interfaces *ifaces);
void hostapd_global_deinit(void);
const char * hostapd_msg_ifname_cb(void *ctx);

#endif /* INIT_H */
