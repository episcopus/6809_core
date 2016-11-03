#include "types.h"

void core_init();

int abx(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int clr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int com(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int daa(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int dec(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int inc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int lsr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int mul(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int nop(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int notimpl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
