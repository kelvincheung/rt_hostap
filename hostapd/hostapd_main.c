/*
 * hostapd / Example program entrypoint
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
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

#include "utils/includes.h"

#include "utils/common.h"
#include "ap/hostapd.h"
#include "config_file.h"
#include "ctrl_iface.h"
#include "init.h"


int hostapd_main(void)
{
	struct hapd_interfaces interfaces;
	int ret = 1;
	size_t i, j;
	int debug = 0;
	char **bss_config = NULL;
	size_t num_bss_configs = 0;

	os_memset(&interfaces, 0, sizeof(interfaces));
	interfaces.reload_config = hostapd_reload_config;
	interfaces.config_read_cb = hostapd_config_read;
	interfaces.for_each_interface = hostapd_for_each_interface;
	interfaces.ctrl_iface_init = hostapd_ctrl_iface_init;
	interfaces.ctrl_iface_deinit = hostapd_ctrl_iface_deinit;
	interfaces.driver_init = hostapd_driver_init;
	interfaces.global_iface_path = NULL;
	interfaces.global_iface_name = NULL;
	interfaces.global_ctrl_sock = -1;
	interfaces.global_ctrl_dst = NULL;

	wpa_msg_register_ifname_cb(hostapd_msg_ifname_cb);

	/*wpa_debug_setup_stdout();*/

	/*interfaces.count = argc - optind;*/
	if (interfaces.count || num_bss_configs) {
		interfaces.iface = os_calloc(interfaces.count + num_bss_configs,
					     sizeof(struct hostapd_iface *));
		if (interfaces.iface == NULL) {
			wpa_printf(MSG_ERROR, "malloc failed");
			return -1;
		}
	}

	if (hostapd_global_init(&interfaces)) {
		wpa_printf(MSG_ERROR, "Failed to initialize global context");
		return -1;
	}

	/* Allocate and parse configuration for full interface files */
	for (i = 0; i < interfaces.count; i++) {
		/*interfaces.iface[i] = hostapd_interface_init(&interfaces,
							     argv[optind + i],
							     debug);*/
		if (!interfaces.iface[i]) {
			wpa_printf(MSG_ERROR, "Failed to initialize interface");
			goto out;
		}
	}

	/* Allocate and parse configuration for per-BSS files */
	for (i = 0; i < num_bss_configs; i++) {
		struct hostapd_iface *iface;
		char *fname;

		wpa_printf(MSG_INFO, "BSS config: %s", bss_config[i]);
		fname = os_strchr(bss_config[i], ':');
		if (fname == NULL) {
			wpa_printf(MSG_ERROR,
				   "Invalid BSS config identifier '%s'",
				   bss_config[i]);
			goto out;
		}
		*fname++ = '\0';
		iface = hostapd_interface_init_bss(&interfaces, bss_config[i],
						   fname, debug);
		if (iface == NULL)
			goto out;
		for (j = 0; j < interfaces.count; j++) {
			if (interfaces.iface[j] == iface)
				break;
		}
		if (j == interfaces.count) {
			struct hostapd_iface **tmp;
			tmp = os_realloc_array(interfaces.iface,
					       interfaces.count + 1,
					       sizeof(struct hostapd_iface *));
			if (tmp == NULL) {
				hostapd_interface_deinit_free(iface);
				goto out;
			}
			interfaces.iface = tmp;
			interfaces.iface[interfaces.count++] = iface;
		}
	}

	/*
	 * Enable configured interfaces. Depending on channel configuration,
	 * this may complete full initialization before returning or use a
	 * callback mechanism to complete setup in case of operations like HT
	 * co-ex scans, ACS, or DFS are needed to determine channel parameters.
	 * In such case, the interface will be enabled from eloop context within
	 * hostapd_global_run().
	 */
	interfaces.terminate_on_error = interfaces.count;
	for (i = 0; i < interfaces.count; i++) {
		if (hostapd_driver_init(interfaces.iface[i]) ||
		    hostapd_setup_interface(interfaces.iface[i]))
			goto out;
	}

	hostapd_global_ctrl_iface_init(&interfaces);

	if (hostapd_global_run(&interfaces)) {
		wpa_printf(MSG_ERROR, "Failed to start eloop");
		goto out;
	}

	ret = 0;

 out:
	hostapd_global_ctrl_iface_deinit(&interfaces);
	/* Deinitialize all interfaces */
	for (i = 0; i < interfaces.count; i++) {
		if (!interfaces.iface[i])
			continue;
		interfaces.iface[i]->driver_ap_teardown =
			!!(interfaces.iface[i]->drv_flags &
			   WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT);
		hostapd_interface_deinit_free(interfaces.iface[i]);
	}
	os_free(interfaces.iface);

	hostapd_global_deinit();

	os_free(bss_config);

	return ret;
}
