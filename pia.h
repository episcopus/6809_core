struct pia_state_struct {
    /* Only capture state here for the addresses that are multiplexed, i.e. the
       data and data direction registers. The control register state is mastered
       in the memory array */
    uint8 ddr_1_a;
    uint8 dddr_1_a;
    uint8 cr_1_a;
    uint8 ddr_1_b;
    uint8 dddr_1_b;
    uint8 cr_1_b;
    uint8 ddr_2_a;
    uint8 dddr_2_a;
    uint8 cr_2_a;
    uint8 ddr_2_b;
    uint8 dddr_2_b;
    uint8 cr_2_b;

    uint8 host_keys[56];
};

/* Memory handler routine for PIA address space 0xFF00-0xFF03, 0xFF20-0xFF23 */
uint8 pia_read_byte_from_memory(uint16 address);
void pia_write_byte_to_memory(uint16 address, uint8 byte);
void pia_update_keyboard();

/* Coco keyboard keys PIA id's follow */
#define PIA_KEYBOARD_SIZE 56
#define PIA_KEY_AT    0
#define PIA_KEY_A     1
#define PIA_KEY_B     2
#define PIA_KEY_C     3
#define PIA_KEY_D     4
#define PIA_KEY_E     5
#define PIA_KEY_F     6
#define PIA_KEY_G     7
#define PIA_KEY_H     8
#define PIA_KEY_I     9
#define PIA_KEY_J     10
#define PIA_KEY_K     11
#define PIA_KEY_L     12
#define PIA_KEY_M     13
#define PIA_KEY_N     14
#define PIA_KEY_O     15
#define PIA_KEY_P     16
#define PIA_KEY_Q     17
#define PIA_KEY_R     18
#define PIA_KEY_S     19
#define PIA_KEY_T     20
#define PIA_KEY_U     21
#define PIA_KEY_V     22
#define PIA_KEY_W     23
#define PIA_KEY_X     24
#define PIA_KEY_Y     25
#define PIA_KEY_Z     26
#define PIA_KEY_UP    27
#define PIA_KEY_DOWN  28
#define PIA_KEY_LEFT  29
#define PIA_KEY_RIGHT 30
#define PIA_KEY_SPACE 31
#define PIA_KEY_0     32
#define PIA_KEY_1     33
#define PIA_KEY_2     34
#define PIA_KEY_3     35
#define PIA_KEY_4     36
#define PIA_KEY_5     37
#define PIA_KEY_6     38
#define PIA_KEY_7     39
#define PIA_KEY_8     40
#define PIA_KEY_9     41
#define PIA_KEY_COLON 42
#define PIA_KEY_SEMIC 43
#define PIA_KEY_COMMA 44
#define PIA_KEY_MINUS 45
#define PIA_KEY_PER   46
#define PIA_KEY_SLASH 47
#define PIA_KEY_ENTER 48
#define PIA_KEY_CLEAR 49
#define PIA_KEY_BREAK 50
#define PIA_KEY_SHIFT 55
