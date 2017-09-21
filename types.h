typedef unsigned short int uint16;
typedef unsigned char uint8;
typedef unsigned int uint32;

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
    unsigned int c : 1; /* Carry (or borrow) */
    unsigned int v : 1; /* Overflow */
    unsigned int z : 1; /* Zero result */
    unsigned int n : 1; /* Negative result (twos complement) */
    unsigned int i : 1; /* IRQ interrupt masked */
    unsigned int h : 1; /* Half-Carry */
    unsigned int f : 1; /* FIRQ interrupt masked */
    unsigned int e : 1; /* Entire register state stacked */
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
    REG_B,
    REG_CC
};

enum reg_size {
    REG_SIZE_8,
    REG_SIZE_16,
    REG_SIZE_INVALID
};

typedef int (*opcode_func)(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);

/* Opcode structure definition for opcode_table.c */
struct opcode_def {
    char* instruction;
    uint8 opcode;
    uint8 cycle_count;
    enum target_register t_r;
    enum addressing_mode mode;
    opcode_func func;
};

struct mem_loader_def {
    uint16 root_address;
    uint8* byte_payload;
    uint16 byte_size;
};

struct stack_op_postbyte_entry {
    enum target_register reg;
    uint8 bit;
};

struct test_check {
    uint16 address;
    uint8 value;
};
