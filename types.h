typedef unsigned short int uint16;
typedef unsigned char uint8;

#define FALSE 0
#define TRUE 1

/* This union manages the differing endinaness of the Intel
   CPU (little endian) vs the emulated MC6809E (big endian).
   Setting a value to the b register will end up flipping it to
   the lower part of the d register as it should be. */
typedef union {
    uint16 d;
    struct {
        uint8 b;
        uint8 a;
    } byte_acc;
} d_register;

/* This bit field is arranged from low order bit (c) to high
   order bit (e) on Intel little endian CPU's. */
struct condition_code {
    unsigned int c : 1;
    unsigned int v : 1;
    unsigned int z : 1;
    unsigned int n : 1;
    unsigned int i : 1;
    unsigned int h : 1;
    unsigned int f : 1;
    unsigned int e : 1;
};

struct cpu_state {
    uint16 x;
    uint16 y;
    uint16 u;
    uint16 s;
    uint16 pc;
    d_register d;
    uint8 dp;
    struct condition_code cc;
};

enum addressing_mode {
    IMMEDIATE,
    DIRECT,
    INDEXED,
    EXTENDED,
    INHERENT
};

enum target_register {
    REG_NONE,
    REG_X,
    REG_Y,
    REG_U,
    REG_S,
    REG_PC,
    REG_D,
    REG_DP,
    REG_A,
    REG_B
};

typedef int (*opcode_func)(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);

struct opcode_def {
    char* instruction;
    uint8 opcode;
    uint8 cycle_count;
    enum addressing_mode mode;
    opcode_func func;
};
