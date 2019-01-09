/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.h - Configuration routines
 */

#ifndef CONFIG_H
#define CONFIG_H

#define IO_MODE_DWBECKER      0
#define IO_MODE_DWBITBANGER   1
#define IO_MODE_RS232         2
#define IO_MODE_SERIAL        16
#define IO_MODE_ETHERNET      32

#define XON_THRESHOLD 46
#define XOFF_THRESHOLD 127

typedef struct configInfo
{
  unsigned char valid;
  unsigned char io_mode;
  unsigned char baud;
  unsigned char use_dhcp;
  unsigned long ip_address;
  unsigned long netmask;
  unsigned long gateway;
  unsigned long dns;
  unsigned char driver_ser[16];
  unsigned char driver_mou[16];
  unsigned char color_foreground;
  unsigned char color_background;
  unsigned char color_border;
  unsigned short xon_threshold;
  unsigned short xoff_threshold;
  unsigned char fill;
} ConfigInfo;

extern ConfigInfo config;

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void);

/**
 * config_init_hook()
 * Special system specific initialization functions.
 */
void config_init_hook(void);

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void);

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void);

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void);

#endif /* CONFIG_H */
