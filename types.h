typedef unsigned short int uint16;
typedef unsigned char uint8;

typedef union {
    uint16 d;
    struct {
        uint8 b;
        uint8 a;
    } b;
} d_register;

struct cpu_state {
    uint16 x;
    uint16 y;
    uint16 u;
    uint16 s;
    uint16 pc;
    d_register d;
};
