#include "core.h"

int extended(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);

int abx(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int adc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int add(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int andcc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int addd(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int and(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int asr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int bit(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int clr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int cmp(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int cmp16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int com(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int daa(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int dec(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int eor(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int exg(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int inc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int ld(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int ld16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int lsr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int mul(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int neg(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int nop(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int notimpl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int or(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int orcc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int pul(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int psh(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int rol(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int ror(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int sbc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int sex(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int sub(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
int sub16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
/* int st(uint8 opcode, enum target_register t_r, enum addressing_mode a_m); */
int tst(uint8 opcode, enum target_register t_r, enum addressing_mode a_m);
