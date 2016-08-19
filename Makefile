ifeq ($(CONFIG_OS),nuttx)
-include $(TOPDIR)/.config
CFLAGS += -I$(TOPDIR)/include
CFLAGS += -I$(TOPDIR)/hardware/id.bsp/include
export CFLAGS CONFIG_OS
endif

all:
ifeq ($(CONFIG_OS),nuttx)
ifeq ($(CONFIG_WIFI_STA),y)
	$(Q) $(MAKE) -C wpa_supplicant
endif
ifeq ($(CONFIG_WIFI_AP),y)
	$(Q) $(MAKE) -C hostapd
endif
	$(Q) cp -r libwifi.a $(TOPDIR)/lib/
else
	$(Q) $(MAKE) -C wpa_supplicant clean
	$(Q) $(MAKE) -C wpa_supplicant wpa_supplicant
	$(Q) $(MAKE) -C hostapd clean
	$(Q) $(MAKE) -C hostapd hostapd
endif

clean:
ifeq ($(CONFIG_OS),nuttx)
ifeq ($(CONFIG_WIFI_STA),y)
	$(Q) $(MAKE) -C wpa_supplicant clean
endif
ifeq ($(CONFIG_WIFI_AP),y)
	$(Q) $(MAKE) -C hostapd clean
endif
else
	$(Q) $(MAKE) -C wpa_supplicant clean
	$(Q) $(MAKE) -C hostapd clean
endif

distclean:
ifeq ($(CONFIG_OS),nuttx)
ifeq ($(CONFIG_WIFI_STA),y)
	$(Q) $(MAKE) -C wpa_supplicant distclean
endif
ifeq ($(CONFIG_WIFI_AP),y)
	$(Q) $(MAKE) -C hostapd distclean
endif
else
	$(Q) $(MAKE) -C wpa_supplicant distclean
	$(Q) $(MAKE) -C hostapd distclean
endif
