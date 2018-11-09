
struct ser_params {
    unsigned char       baudrate;       /* Baudrate */
    unsigned char       databits;       /* Number of data bits */
    unsigned char       stopbits;       /* Number of stop bits */
    unsigned char       parity;         /* Parity setting */
    unsigned char       handshake;      /* Type of handshake to use */
};


#define SER_BAUD_38400 0
#define SER_BAUD_300 0
#define SER_BAUD_1200 0
#define SER_BAUD_2400 0
#define SER_BAUD_9600 0
#define SER_BAUD_19200 0
#define SER_BITS_8     0
#define SER_PAR_NONE   0
#define SER_HS_HW      0
#define SER_STOP_1     0
#define SER_ERR_OK     0
#define SER_ERR_NO_DATA 0
