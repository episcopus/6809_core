#define FALSE 0
#define TRUE 1

/* Allocate 64K of memory by default */
#define MEMORY_SIZE 0x10000

/* Start user space at 0x2000 given other reserved things in the CoCo
   memory map before it. This leaves 24K of usable user space under 0x7FFF. */
#define USER_SPACE_ROOT 0x2000

/* Pin the s and u stack pointers at 4KB intervals from the user space root */
#define S_POINTER 0x3000
#define U_POINTER 0x4000

/* For cmocka, replace regular assert with mock_assert() which allows us to
   test for assert()'s in tests. */
/* #ifdef UNIT_TESTING */
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);

#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
/* #endif */

#define OP_NEG_D 0x0
#define OP_COM_D 0x3
#define OP_ROR_D 0x6
#define OP_ASR_D 0x7
#define OP_ASL_D 0x8
#define OP_ROL_D 0x9
#define OP_DEC_D 0xA
#define OP_INC_D 0xC
#define OP_JMP_D 0xE
#define OP_CLR 0xF
#define OP_EXTENDED_X10 0x10
#define OP_EXTENDED_X11 0x11
#define OP_NOP 0x12
#define OP_DAA 0x19
#define OP_ORCC 0x1A
#define OP_ANDCC 0x1C
#define OP_SEX 0x1D
#define OP_EXG 0x1E
#define OP_TFR 0x1F
#define OP_PSHS 0x34
#define OP_PULS 0x35
#define OP_PSHU 0x36
#define OP_PULU 0x37
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
#define OP_SUBA 0x80
#define OP_CMPA 0x81
#define OP_SBCA 0x82
#define OP_SUBD 0x83
#define OP_ANDA 0x84
#define OP_BITA 0x85
#define OP_LDA 0x86
#define OP_EORA 0x88
#define OP_ADCA 0x89
#define OP_ORA 0x8A
#define OP_ADDA 0x8B
#define OP_CMPX 0x8C
#define OP_LDX 0x8E
#define OP_CMPA_D 0x91
#define OP_SBCA_D 0x92
#define OP_ANDA_D 0x94
#define OP_BITA_D 0x95
#define OP_LDA_D 0x96
#define OP_STA_D 0X97
#define OP_EORA_D 0x98
#define OP_ADCA_D 0x99
#define OP_ORA_D 0x9A
#define OP_ADDA_D 0x9B
#define OP_CMPX_D 0x9C
#define OP_JSR_D 0x9D
#define OP_LDX_D 0x9E
#define OP_STX_D 0x9F
#define OP_SUBB 0xC0
#define OP_CMPB 0xC1
#define OP_SBCB 0xC2
#define OP_ADDD 0xC3
#define OP_ANDB 0xC4
#define OP_BITB 0xC5
#define OP_LDB 0xC6
#define OP_EORB 0xC8
#define OP_ADCB 0xC9
#define OP_ORB 0xCA
#define OP_ADDB 0xCB
#define OP_LDD 0xCC
#define OP_LDU 0xCE
#define OP_CMPB_D 0xD1
#define OP_SBCB_D 0xD2
#define OP_ADDD_D 0xD3
#define OP_LDB_D 0xD6
#define OP_STB_D 0xD7
#define OP_LDD_D 0xDC
#define OP_BITB_D 0x95
#define OP_ANDB_D 0xD4
#define OP_STB 0xD7
#define OP_EORB_D 0xD8
#define OP_ADCB_D 0xD9
#define OP_ORB_D 0xDA
#define OP_ADDB_D 0xDB
#define OP_STD_D 0xDD
#define OP_LDU_D 0xDE
#define OP_STU_D 0xDF

/* Two byte opcodes - those have 0x10 as leading byte */

#define OP_CMPD 0x83
#define OP_CMPD_D 0x93
#define OP_CMPY_D 0x9C
#define OP_LDY 0x8E
#define OP_CMPY 0x8C
#define OP_LDY_D 0x9E
#define OP_LDS 0xCE
#define OP_LDS_D 0xDE
#define OP_STS_D 0xDF

/* Two byte opcodes - those have 0x11 as leading byte */

#define OP_CMPS 0x8C
#define OP_CMPU 0x83
#define OP_CMPU_D 0x93
#define OP_CMPS_D 0x9C
