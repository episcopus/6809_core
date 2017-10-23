struct sam_state_struct {
    /* Memory map mode */
    uint8 ty_control_bit;

    /* Memory size */
    uint8 m1_control_bit;
    uint8 m0_control_bit;

    /* MPU cycle rate */
    uint8 r1_control_bit;
    uint8 r0_control_bit;

    /* Page number */
    uint8 p1_control_bit;

    /* Video display starting address */
    uint8 f6_control_bit;
    uint8 f5_control_bit;
    uint8 f4_control_bit;
    uint8 f3_control_bit;
    uint8 f2_control_bit;
    uint8 f1_control_bit;
    uint8 f0_control_bit;

    /* Video display mode */
    uint8 v2_control_bit;
    uint8 v1_control_bit;
    uint8 v0_control_bit;
};

struct cpu_state {
    uint16 x; /* Index Register */
    uint16 y; /* Index Register */
    uint16 u; /* User Stack Pointer */
    uint16 s; /* System Stack Pointer */
    uint16 pc; /* Program Counter */
    d_register d; /* Accumulator */
    uint8 dp; /* Direct Page Register */
    struct condition_code cc; /* Condition Codes Register */

    struct sam_state_struct sam_state;
    struct pia_state_struct pia_state;
    struct memory_handler_struct memory_handler;
    struct vdg_state_struct vdg_state;
    uint8* memory; /* Memory map */
    uint8* color_basic;
    uint8* extended_basic;

    uint32 cycle_count;
    /* Global counters tracking remaining amount of cycles until respective
       interval */
    int hsync_cycles;
    int vsync_cycles;

    /* Interrupts, are !=0 when the line is low, i.e. interrupt is active */
    uint8 irq;
    uint8 firq;
    uint8 nmi;
    uint8 halted_state;

    /* Set this to true to break in the monitor during the SWI instruction */
    uint8 swi_hook;
    uint8 swi_hook_set;
};

void core_init(void);
void load_roms(void);
void core_destroy(void);

enum reg_size get_reg_size(enum target_register reg);
uint8 get_reg_value_8(enum target_register reg);
uint16 get_reg_value_16(enum target_register reg);
void set_reg_value_8(enum target_register reg, uint8 value);
void set_reg_value_16(enum target_register reg, uint16 value);
enum reg_size get_reg_size(enum target_register reg);

int load_memory(struct mem_loader_def* defs, uint8 num_defs);
uint8 read_byte_from_memory(uint16 address);
void write_byte_to_memory(uint16 address, uint8 byte);
uint16 read_word_from_memory(uint16 address);
void write_word_to_memory(uint16 address, uint16 word);
uint8 read_byte_handler(enum addressing_mode am, uint8* out_extra_cycles);
void write_byte_handler(enum addressing_mode am, uint8 byte, uint8* out_extra_cycles);
uint16 read_word_handler(enum addressing_mode am, uint8* out_extra_cycles);
void write_word_handler(enum addressing_mode am, uint16 word, uint8* out_extra_cycles);
uint16 get_memory_address_from_postbyte(enum addressing_mode am, uint8* out_extra_cycles);

void push_byte_to_stack(enum target_register t_r, uint8 data);
uint8 pull_byte_from_stack(enum target_register t_r);
void push_word_to_stack(enum target_register t_r, uint16 data);
uint16 pull_word_from_stack(enum target_register t_r);
uint8 push_registers_to_stack(uint8 reg_field, enum target_register reg_stack);
uint8 pull_registers_from_stack(uint8 reg_field, enum target_register reg_stack);

uint16 decode_indexed_addressing_postbyte(uint8* out_extra_cycles);
enum target_register decode_register_from_indexed_postbyte(uint8 postbyte);
uint16 decode_constant_offset_postbyte(uint8* out_extra_cycles);
uint16 decode_accumulator_offset_postbyte(uint8* out_extra_cycles);
uint16 decode_inc_dec_offset_postbyte(uint8* out_extra_cycles);
uint16 decode_constant_offset_from_pc(uint8* out_extra_cycles);
uint16 decode_extended_indirect(uint8* out_extra_cycles);
void decode_source_target_postbyte(uint8 postbyte, enum target_register* out_source, enum target_register* out_target);

uint8 disassemble_instruction(uint16 pc, char* decoded);
uint8 disassemble_indexed_addressing_postbyte(uint16 pc, char* decoded);
uint8 disassemble_constant_offset_postbyte(uint16 pc, char* decoded);
uint8 disassemble_extended_indirect(uint16 pc, char* decoded);

uint16 init_from_decb_memory(const uint8* buffer, uint16 buffer_size);
uint16 init_from_decb_file(const char* filename);

uint32 run_cycles(uint32 wanted_cycles);
uint32 run_hsync_interval(void);
void perform_tick_housekeeping(uint32 cycles);
void perform_hsync_housekeeping(uint32 cycles);
void perform_vsync_housekeeping(uint32 cycles);
uint32 process_interrupts(void);
uint32 process_swi(enum swi_type swi);
