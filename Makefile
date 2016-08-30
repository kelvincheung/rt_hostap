ifdef CONFIG_OS
ifeq ($(CONFIG_OS),nuttx)
-include $(TOPDIR)/.config
CFLAGS += -I$(TOPDIR)/include
CFLAGS += -I$(TOPDIR)/hardware/id.bsp/include
else ifeq ($(CONFIG_OS),sx)
CFLAGS +=  -D__SX__
endif
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
else ifeq ($(CONFIG_OS),sx)
	$(Q) $(MAKE) -C wpa_supplicant
	$(Q) $(MAKE) -C hostapd
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
