#define MOUSE_ERR_OK 0
#define MOUSE_BTN_LEFT 0
#define MOUSE_BTN_RIGHT 0

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

extern const struct mouse_callbacks mouse_def_callbacks;

