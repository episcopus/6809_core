#include "types.h"

void core_init();
void core_destroy();

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

uint16 decode_indexed_addressing_postbyte();
enum target_register decode_register_from_indexed_postbyte(uint8 postbyte);
uint16 decode_constant_offset_postbyte(uint8* out_extra_cycles);
uint16 decode_accumulator_offset_postbyte(uint8* out_extra_cycles);

uint32 run_cycles(uint32 wanted_cycles);
void decode_source_target_postbyte(uint8 postbyte, enum target_register* out_source, enum target_register* out_target);

enum reg_size get_reg_size(enum target_register reg);
uint8 get_reg_value_8(enum target_register reg);
uint16 get_reg_value_16(enum target_register reg);
void set_reg_value_8(enum target_register reg, uint8 value);
void set_reg_value_16(enum target_register reg, uint16 value);
enum reg_size get_reg_size(enum target_register reg);
