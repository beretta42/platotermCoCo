#define MOUSE_ERR_OK 0
#define MOUSE_BTN_LEFT 1
#define MOUSE_BTN_RIGHT 0


extern mouse_x;
extern mouse_y;
extern mouse_b;

struct mouse_callbacks {
    void (*hide) (void);
    void (*show) (void);
    void (*prep) (void);
    void (*draw) (void);
    void (*movex) (int x);
    void (*movey) (int y);
};

struct mouse_pos {
    int                 x;
    int                 y;
};
struct mouse_info {
    struct mouse_pos    pos;           /* Mouse position */
    unsigned char       buttons;       /* Mouse button mask */
};

void mouse_move (int x, int y);
void mouse_hide (void);
void mouse_show (void);

extern const struct mouse_callbacks mouse_def_callbacks;

