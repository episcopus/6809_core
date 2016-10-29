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
    uint8 dp;
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
