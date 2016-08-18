/*
 * hostapd / Configuration file parser
 * Copyright (c) 2016, Kelvin Cheung <keguang.zhang@gmail.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "utils/includes.h"

#include "utils/common.h"
#include "drivers/driver.h"
#include "ap/ap_config.h"

/**
 * hostapd_config_read - Read and parse a configuration file
 * @fname: Configuration file name (including path, if needed)
 * Returns: Allocated configuration data structure
 */
struct hostapd_config * hostapd_config_read(const char *fname)
{
	struct hostapd_config *conf;
	int errors = 0;
	size_t i;

	conf = hostapd_config_defaults();
	if (conf == NULL)
		return NULL;

	/* set default driver based on configuration */
	conf->driver = wpa_drivers[0];
	if (conf->driver == NULL) {
		wpa_printf(MSG_ERROR, "No driver wrappers registered!");
		hostapd_config_free(conf);
		return NULL;
	}

	conf->last_bss = conf->bss[0];

	for (i = 0; i < conf->num_bss; i++)
		hostapd_set_security_params(conf->bss[i], 1);

	if (hostapd_config_check(conf, 1))
		errors++;

#ifndef WPA_IGNORE_CONFIG_ERRORS
	if (errors) {
		wpa_printf(MSG_ERROR, "%d errors found in configuration file "
			   "'%s'", errors, fname);
		hostapd_config_free(conf);
		conf = NULL;
	}
#endif /* WPA_IGNORE_CONFIG_ERRORS */

	return conf;
}
