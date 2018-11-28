

#define COLOR_BLACK 0x00
#define COLOR_WHITE 0x3f

#define TGI_COLOR_BLACK COLOR_BLACK
#define TGI_COLOR_WHITE COLOR_WHITE


unsigned char tgi_getcolor (void);
void tgi_setcolor (unsigned char color);
void tgi_bar (int x1, int y1, int x2, int y2);
void tgi_init (void);
void tgi_clear (void);
void tgi_setpixel (int x, int y);
void tgi_line (int x1, int y1, int x2, int y2);
void tgi_hline (int x1, int y1, int x2);
void tgi_done (void);
void tgi_uninstall (void);

void tgi_cset(unsigned char color);
void tgi_char_blit(int x, int y, unsigned char c);
void tgi_char_blit_erase(int x, int y, unsigned char c);
void tgi_char_blit_rewrite(int x, int y, unsigned char c);
void tgi_hline(int x, int y, int w);
