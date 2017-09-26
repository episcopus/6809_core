struct pia_state_struct {
    /* Only capture state here for the addresses that are multiplexed, i.e. the
       data and data direction registers. The control register state is mastered
       in the memory array */
    uint8 ddr_1_a;
    uint8 dddr_1_a;
    uint8 ddr_1_b;
    uint8 dddr_1_b;
    uint8 ddr_2_a;
    uint8 dddr_2_a;
    uint8 ddr_2_b;
    uint8 dddr_2_b;
};

/* Memory handler routine for PIA address space 0xFF00-0xFF03, 0xFF20-0xFF23 */
uint8 pia_read_byte_from_memory(uint16 address);
void pia_write_byte_to_memory(uint16 address, uint8 byte);
