
#define SER_ERR_OK     0
#define SER_ERR_NO_DATA 1

unsigned char ser_load_driver (const char* driver);
unsigned char ser_unload (void);
unsigned char ser_uninstall (void);
unsigned char ser_close (void);
unsigned char ser_open (ConfigInfo *params);
unsigned char ser_ioctl (unsigned char code, void* data);
unsigned char ser_get (char* b);
unsigned char ser_put (char b);
unsigned char ser_put_clean (char b);

extern unsigned char enable_poll;
