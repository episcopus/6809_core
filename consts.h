#define FALSE 0
#define TRUE 1

/* Allocate 64K of memory by default */
#define MEMORY_SIZE 0x10000

/* Start user space at 0x2000 given other reserved things in the CoCo
   memory map before it. This leaves 24K of usable user space under 0x7FFF. */
#define USER_SPACE_ROOT 0x2000

/* For cmocka, replace regular assert with mock_assert() which allows us to
   test for assert()'s in tests. */
/* #ifdef UNIT_TESTING */
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);

#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
/* #endif */

#define OP_EXTENDED_X10 0x10
#define OP_EXTENDED_X11 0x11
#define OP_NOP 0x12
#define OP_DAA 0x19
#define OP_ANDCC 0x1C
#define OP_SEX 0x1D
#define OP_ABX 0x3A
#define OP_MUL 0x3D
#define OP_NEGA 0x40
#define OP_COMA 0x43
#define OP_LSRA 0x44
#define OP_RORA 0x46
#define OP_ASRA 0x47
#define OP_ASLA 0x48
#define OP_ROLA 0x49
#define OP_DECA 0x4A
#define OP_INCA 0x4C
#define OP_TSTA 0x4D
#define OP_CLRA 0x4F
#define OP_NEGB 0x50
#define OP_COMB 0x53
#define OP_LSRB 0x54
#define OP_RORB 0x56
#define OP_ASRB 0x57
#define OP_ASLB 0x58
#define OP_ROLB 0x59
#define OP_DECB 0x5A
#define OP_INCB 0x5C
#define OP_TSTB 0x5D
#define OP_CLRB 0x5F
#define OP_CMPA 0x81
#define OP_ANDA 0x84
#define OP_BITA 0x85
#define OP_ADCA 0x89
#define OP_ADDA 0x8B
#define OP_CMPX 0x8C
#define OP_CMPB 0xC1
#define OP_ADDD 0xC3
#define OP_ANDB 0xC4
#define OP_BITB 0xC5
#define OP_ADCB 0xC9
#define OP_ADDB 0xCB

/* Two byte opcodes - those have 0x10 as leading byte */

#define OP_CMPD 0x83
#define OP_CMPY 0x8C

/* Two byte opcodes - those have 0x11 as leading byte */

#define OP_CMPS 0x8C
#define OP_CMPU 0x83
