#include "types.h"

void core_init();
void core_destroy();
int load_memory(struct mem_loader_def* defs, uint8 num_defs);
uint8 read_byte_from_memory(uint16 address);
void write_byte_to_memory(uint16 address, uint8 byte);
uint16 read_word_from_memory(uint16 address);
void write_word_to_memory(uint16 address, uint16 word);
uint8 read_byte_handler(enum addressing_mode am);
uint16 read_word_handler(enum addressing_mode am);
uint32 run_cycles(uint32 wanted_cycles);

int abx(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int adc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int add(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int andcc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int addd(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int and(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int clr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int com(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int daa(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int dec(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int inc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int lsr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int mul(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int neg(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int nop(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int notimpl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int rol(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int ror(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int sex(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int tst(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
