/* Core tests */
int test_setup(void **state);
int test_teardown(void **state);
void core_init_test(void **state);
void test_e_flag(void **state);
void test_f_flag(void **state);
void test_h_flag(void **state);
void test_i_flag(void **state);
void test_n_flag(void **state);
void test_z_flag(void **state);
void test_v_flag(void **state);
void test_c_flag(void **state);
void test_all_flags(void **state);
void test_load_memory(void **state);
void test_load_memory_too_big(void **state);
void test_load_memory_too_far(void **state);
void read_byte_from_memory_test(void **state);
void read_word_from_memory_test(void **state);
void read_byte_handler_immedidate_test(void **state);
void read_byte_handler_direct_test(void **state);
void read_byte_handler_extended_test(void **state);
void read_word_handler_immedidate_test(void **state);
void read_word_handler_direct_test(void **state);
void read_word_handler_extended_test(void **state);
void memory_clear_test(void **state);
void get_memory_address_from_postbyte_direct_test(void **state);
void get_memory_address_from_postbyte_immediate_test(void **state);
void get_memory_address_from_postbyte_extended_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_basic_x_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_basic_y_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_basic_u_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_basic_s_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_5bit_x_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_5bit_positive_x_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_8bit_y_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_16bit_s_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_16bit_s_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_8bit_s_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_constant_indirect_u_test(void **state);
void get_memory_address_from_postbyte_indexed_accumulator_a_test(void **state);
void get_memory_address_from_postbyte_indexed_accumulator_b_test(void **state);
void get_memory_address_from_postbyte_indexed_accumulator_d_test(void **state);
void get_memory_address_from_postbyte_indexed_accumulator_a_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_accumulator_b_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_accumulator_d_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_inc_1_test(void **state);
void get_memory_address_from_postbyte_indexed_inc_2_test(void **state);
void get_memory_address_from_postbyte_indexed_dec_1_test(void **state);
void get_memory_address_from_postbyte_indexed_dec_2_test(void **state);
void get_memory_address_from_postbyte_indexed_inc_2_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_dec_2_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_pc_offset_8_test(void **state);
void get_memory_address_from_postbyte_indexed_pc_offset_16_test(void **state);
void get_memory_address_from_postbyte_indexed_pc_offset_8_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_pc_offset_16_indirect_test(void **state);
void get_memory_address_from_postbyte_indexed_extended_indirect_test(void **state);
void run_cycles_test(void **state);
void run_cycles_multiple_test(void **state);
void run_cycles_notimpl_test(void **state);
void process_interrupts_nmi_test(void **state);
void process_interrupts_nmi_with_rti_test(void **state);
void process_interrupts_firq_test(void **state);
void process_interrupts_firq_with_rti_test(void **state);
void process_interrupts_irq_test(void **state);
void process_interrupts_irq_with_rti_test(void **state);
void decode_source_target_postbyte_test(void **state);
void decode_source_target_postbyte_2_test(void **state);
void decode_source_target_postbyte_invalid_test(void **state);
void get_reg_value_8_test(void **state);
void get_reg_value_8_invalid_test(void **state);
void set_reg_value_8_test(void **state);
void get_reg_value_16_test(void **state);
void get_reg_value_16_invalid_test(void **state);
void set_reg_value_16_test(void **state);

void push_byte_to_stack_basic_test(void **state);
void pull_byte_from_stack_basic_test(void **state);
void push_byte_to_stack_invalid_test(void **state);
void push_pull_byte_stack_combo_test(void **state);
void push_byte_to_stack_u_test(void **state);
void push_word_to_stack_basic_test(void **state);
void pull_word_from_stack_basic_test(void **state);
void push_word_to_stack_invalid_test(void **state);
void push_pull_word_stack_combo_test(void **state);
void push_word_to_stack_u_test(void **state);

/* Memory handler tests */
void sam_read_byte_test(void **state);
void sam_write_byte_test(void **state);
void sam_memory_handler_byte_from_core_test(void **state);
void sam_not_redirected_byte_test(void **state);
void sam_redirected_byte_test(void **state);
void sam_irq_redirected_byte_test(void **state);
void sam_irq_redirected_byte_test_2(void **state);

/* Immediate tests */
void adca_immediate_test(void **state);
void adcb_immediate_test(void **state);
void adca_immediate_carry_test(void **state);
void adcb_immediate_carry_test(void **state);
void adca_immediate_overflow_test(void **state);
void adcb_immediate_overflow_test(void **state);
void adca_immediate_carry_flag_test(void **state);
void adcb_immediate_carry_flag_test(void **state);
void adca_immediate_half_carry_test(void **state);
void adcb_immediate_half_carry_test(void **state);
void adca_immediate_zero_test(void **state);
void adcb_immediate_zero_test(void **state);
void adda_immediate_test(void **state);
void addb_immediate_test(void **state);
void adda_immediate_carry_test(void **state);
void addb_immediate_carry_test(void **state);
void adda_immediate_overflow_test(void **state);
void addb_immediate_overflow_test(void **state);
void adda_immediate_carry_flag_test(void **state);
void addb_immediate_carry_flag_test(void **state);
void adda_immediate_half_carry_test(void **state);
void addb_immediate_half_carry_test(void **state);
void adda_immediate_zero_test(void **state);
void addb_immediate_zero_test(void **state);
void addd_immediate_test(void **state);
void addd_immediate_overflow_test(void **state);
void addd_immediate_carry_flag_test(void **state);
void addd_immediate_zero_test(void **state);
void anda_immediate_test(void **state);
void andb_immediate_test(void **state);
void anda_immediate_zero_test(void **state);
void andb_immediate_zero_test(void **state);
void andcc_immediate_test(void **state);
void andcc_immediate_e_test(void **state);
void bita_immediate_test(void **state);
void bitb_immediate_test(void **state);
void bita_immediate_zero_test(void **state);
void bitb_immediate_zero_test(void **state);
void bita_immediate_negative_test(void **state);
void bitb_immediate_negative_test(void **state);
void cmpa_immediate_nocarry(void **state);
void cmpb_immediate_nocarry(void **state);
void cmpa_immediate_carry(void **state);
void cmpb_immediate_carry(void **state);
void cmpa_immediate_overflow(void **state);
void cmpb_immediate_overflow(void **state);
void cmpa_immediate_zero(void **state);
void cmpb_immediate_zero(void **state);
void cmpd_immediate_nocarry(void **state);
void cmpd_immediate_carry(void **state);
void cmpd_immediate_overflow(void **state);
void cmpd_immediate_zero(void **state);
void cmps_immediate_nocarry(void **state);
void cmps_immediate_carry(void **state);
void cmps_immediate_overflow(void **state);
void cmps_immediate_zero(void **state);
void cmpu_immediate_nocarry(void **state);
void cmpu_immediate_carry(void **state);
void cmpu_immediate_overflow(void **state);
void cmpu_immediate_zero(void **state);
void cmpx_immediate_nocarry(void **state);
void cmpx_immediate_carry(void **state);
void cmpx_immediate_overflow(void **state);
void cmpx_immediate_zero(void **state);
void cmpy_immediate_nocarry(void **state);
void cmpy_immediate_carry(void **state);
void cmpy_immediate_overflow(void **state);
void cmpy_immediate_zero(void **state);
void cwai_basic_test(void ** state);
void cwai_basic_irq_test(void ** state);
void eora_immediate_test(void **state);
void eorb_immediate_test(void **state);
void eora_immediate_zero_test(void **state);
void eorb_immediate_zero_test(void **state);
void exg_basic_test(void **state);
void exg_basic_16_test(void **state);
void exg_basic_16_to_8_test(void **state);
void exg_basic_8_to_16_test(void **state);
void exg_a_d_8_to_16_test(void **state);
void exg_cc_x_8_to_16_test(void **state);
void exg_cc_invalid_test(void **state);
void exg_x_invalid_test(void **state);
void lda_basic_test(void **state);
void ldb_basic_test(void **state);
void ldd_basic_test(void **state);
void lds_basic_test(void **state);
void ldu_basic_test(void **state);
void ldx_basic_test(void **state);
void ldy_basic_test(void **state);
void ora_immediate_test(void **state);
void orb_immediate_test(void **state);
void orcc_immediate_test(void **state);
void pshs_immediate_test(void ** state);
void pshu_immediate_test(void ** state);
void pshs_nothing_test(void ** state);
void pshu_nothing_test(void ** state);
void pshs_one_test(void ** state);
void pshu_one_test(void ** state);
void pulu_immediate_test(void ** state);
void puls_immediate_test(void ** state);
void pulu_one_test(void ** state);
void puls_one_test(void ** state);
void pulu_nothing_test(void ** state);
void puls_nothing_test(void ** state);
void rti_basic_test(void ** state);
void rti_basic_firq_test(void ** state);
void rts_basic_test(void ** state);
void sbca_immediate_test(void ** state);
void sbcb_immediate_test(void ** state);
void sbca_carry_test(void ** state);
void sbcb_carry_test(void ** state);
void sbca_overflow_test(void ** state);
void sbcb_overflow_test(void ** state);
void suba_immediate_test(void ** state);
void subb_immediate_test(void ** state);
void suba_carry_test(void ** state);
void subb_carry_test(void ** state);
void suba_overflow_test(void ** state);
void subb_overflow_test(void ** state);
void sub16d_immediate_test(void ** state);
void sub16d_carry_test(void ** state);
void sub16d_overflow_test(void ** state);
void swi_basic_test(void ** state);
void swi_with_rti_test(void ** state);
void swi2_basic_test(void ** state);
void swi2_with_rti_test(void ** state);
void swi3_basic_test(void ** state);
void swi3_with_rti_test(void ** state);
void sync_basic_test(void ** state);
void sync_basic_resume_test(void ** state);
void sync_basic_irq_test(void ** state);
void tfr_basic_test(void **state);
void tfr_basic_16_test(void **state);
void tfr_basic_16_to_8_test(void **state);
void tfr_basic_8_to_16_test(void **state);
void tfr_a_d_8_to_16_test(void **state);
void tfr_cc_x_8_to_16_test(void **state);
void tfr_cc_invalid_test(void **state);
void tfr_x_invalid_test(void **state);

/* Inherent tests */
void nop_test(void **state);
void nop_pc_test(void **state);
void abx_test(void **state);
void abx_test_add(void **state);
void abx_test_unsigned(void **state);
void asla_test(void **state);
void asla_flags_test(void **state);
void aslb_test(void **state);
void aslb_flags_test(void **state);
void asra_test(void **state);
void asra_flags_test(void **state);
void asra_negative_test(void **state);
void asrb_test(void **state);
void asrb_flags_test(void **state);
void asrb_negative_test(void **state);
void clra_flags_test(void **state);
void clrb_flags_test(void **state);
void coma_test(void **state);
void comb_test(void **state);
void coma_zero_test(void **state);
void comb_zero_test(void **state);
void daa_test(void **state);
void daa_not_adjusted_test(void **state);
void deca_test(void **state);
void decb_test(void **state);
void deca_zero_test(void **state);
void decb_zero_test(void **state);
void deca_negative_test(void **state);
void decb_negative_test(void **state);
void inca_test(void **state);
void incb_test(void **state);
void inca_overflow_test(void **state);
void incb_overflow_test(void **state);
void inca_zero_test(void **state);
void incb_zero_test(void **state);
void inca_multiple_pc_test(void **state);
void lsra_test(void **state);
void lsra_flags_test(void **state);
void lsra_negative_test(void **state);
void lsrb_test(void **state);
void lsrb_flags_test(void **state);
void lsrb_negative_test(void **state);
void mul_test(void **state);
void mul_signed_test(void **state);
void mul_zero_test(void **state);
void nega_test(void **state);
void negb_test(void **state);
void nega_minusone_test(void **state);
void negb_minusone_test(void **state);
void rola_test(void **state);
void rolb_test(void **state);
void rola_carry_test(void **state);
void rolb_carry_test(void **state);
void rola_rotate_test(void **state);
void rolb_rotate_test(void **state);
void rora_test(void **state);
void rora_basic_test(void **state);
void rorb_test(void **state);
void rorb_basic_test(void **state);
void rora_carry_test(void **state);
void rorb_carry_test(void **state);
void rora_rotate_test(void **state);
void rorb_rotate_test(void **state);
void sex_test(void **state);
void sex_negative_test(void **state);
void sex_zero_test(void **state);
void tsta_test(void **state);
void tstb_test(void **state);
void tsta_negative_test(void **state);
void tstb_negative_test(void **state);
void tsta_zero_test(void **state);
void tstb_zero_test(void **state);

/* Direct tests */
void adca_direct_test(void **state);
void adcb_direct_test(void **state);
void adda_direct_test(void **state);
void addb_direct_test(void **state);
void addd_direct_test(void **state);
void anda_direct_test(void **state);
void andb_direct_test(void **state);
void asl_direct_test(void **state);
void asr_direct_test(void **state);
void bita_direct_test(void **state);
void bitb_direct_test(void **state);
void clr_direct_test(void **state);
void cmpa_direct_nocarry_test(void **state);
void cmpb_direct_nocarry_test(void **state);
void cmpd_direct_nocarry_test(void **state);
void cmps_direct_nocarry_test(void **state);
void cmpu_direct_nocarry_test(void **state);
void cmpx_direct_overflow_test(void **state);
void cmpy_direct_zero_test(void **state);
void com_direct_test(void **state);
void dec_direct_test(void **state);
void eora_direct_test(void **state);
void eorb_direct_test(void **state);
void inc_direct_test(void **state);
void jmp_direct_test(void **state);
void jsr_direct_test(void **state);
void lda_direct_test(void **state);
void ldb_direct_test(void **state);
void ldd_direct_test(void **state);
void lds_direct_test(void **state);
void ldu_direct_test(void **state);
void ldx_direct_test(void **state);
void ldy_direct_test(void **state);
void lsr_direct_test(void **state);
void neg_direct_test(void **state);
void ora_direct_test(void **state);
void orb_direct_test(void **state);
void rol_direct_test(void **state);
void ror_direct_test(void **state);
void sbca_direct_test(void ** state);
void sbcb_direct_test(void ** state);
void sta_direct_test(void **state);
void stb_direct_test(void **state);
void std_direct_test(void **state);
void sts_direct_test(void **state);
void stu_direct_test(void **state);
void stx_direct_test(void **state);
void sty_direct_test(void **state);
void suba_direct_test(void ** state);
void subb_direct_test(void ** state);
void subd_direct_test(void ** state);
void tst_direct_test(void **state);

/* Extended tests */
void adca_extended_test(void **state);
void adcb_extended_test(void **state);
void adda_extended_test(void **state);
void addb_extended_test(void **state);
void addd_extended_test(void **state);
void anda_extended_test(void **state);
void andb_extended_test(void **state);
void asl_extended_test(void **state);
void asr_extended_test(void **state);
void bita_extended_test(void **state);
void bitb_extended_test(void **state);
void clr_extended_test(void **state);
void cmpa_extended_nocarry_test(void **state);
void cmpb_extended_nocarry_test(void **state);
void cmpd_extended_nocarry_test(void **state);
void cmps_extended_nocarry_test(void **state);
void cmpu_extended_nocarry_test(void **state);
void cmpx_extended_overflow_test(void **state);
void cmpy_extended_zero_test(void **state);
void com_extended_test(void **state);
void lsr_extended_test(void **state);
void dec_extended_test(void **state);
void eora_extended_test(void **state);
void eorb_extended_test(void **state);
void inc_extended_test(void **state);
void jmp_extended_test(void **state);
void jsr_extended_test(void **state);
void lda_extended_test(void **state);
void ldb_extended_test(void **state);
void ldd_extended_test(void **state);
void lds_extended_test(void **state);
void ldu_extended_test(void **state);
void ldx_extended_test(void **state);
void ldy_extended_test(void **state);
void neg_extended_test(void **state);
void ora_extended_test(void **state);
void orb_extended_test(void **state);
void rol_extended_test(void **state);
void ror_extended_test(void **state);
void sbca_extended_test(void ** state);
void sbcb_extended_test(void ** state);
void sta_extended_test(void **state);
void stb_extended_test(void **state);
void std_extended_test(void **state);
void sts_extended_test(void **state);
void stu_extended_test(void **state);
void stx_extended_test(void **state);
void sty_extended_test(void **state);
void suba_extended_test(void ** state);
void subb_extended_test(void ** state);
void subd_extended_test(void ** state);
void tst_extended_test(void **state);

/* Indexed tests */
void adca_indexed_test(void **state);
void adcb_indexed_test(void **state);
void adda_indexed_test(void **state);
void addb_indexed_test(void **state);
void addd_indexed_test(void **state);
void anda_indexed_test(void **state);
void andb_indexed_test(void **state);
void asl_indexed_test(void **state);
void asr_indexed_test(void **state);
void bita_indexed_test(void **state);
void bitb_indexed_test(void **state);
void clr_indexed_test(void **state);
void cmpa_indexed_nocarry_test(void **state);
void cmpb_indexed_nocarry_test(void **state);
void cmpd_indexed_nocarry_test(void **state);
void cmps_indexed_nocarry_test(void **state);
void cmpu_indexed_nocarry_test(void **state);
void cmpx_indexed_overflow_test(void **state);
void cmpy_indexed_zero_test(void **state);
void com_indexed_test(void **state);
void dec_indexed_test(void **state);
void eora_indexed_test(void **state);
void eorb_indexed_test(void **state);
void inc_indexed_test(void **state);
void jmp_indexed_test(void **state);
void jsr_indexed_test(void **state);
void lda_indexed_test(void **state);
void ldb_indexed_test(void **state);
void ldd_indexed_test(void **state);
void lds_indexed_test(void **state);
void ldu_indexed_test(void **state);
void ldx_indexed_test(void **state);
void ldy_indexed_test(void **state);
void leas_indexed_test(void **state);
void leau_indexed_test(void **state);
void leax_indexed_test(void **state);
void leay_indexed_test(void **state);
void lsr_indexed_test(void **state);
void neg_indexed_test(void **state);
void ora_indexed_test(void **state);
void orb_indexed_test(void **state);
void rol_indexed_test(void **state);
void ror_indexed_test(void **state);
void sbca_indexed_test(void ** state);
void sbcb_indexed_test(void ** state);
void sta_indexed_test(void **state);
void stb_indexed_test(void **state);
void std_indexed_test(void **state);
void sts_indexed_test(void **state);
void stu_indexed_test(void **state);
void stx_indexed_test(void **state);
void sty_indexed_test(void **state);
void suba_indexed_test(void ** state);
void subb_indexed_test(void ** state);
void subd_indexed_test(void ** state);
void tst_indexed_test(void **state);

/* Branch tests */
void bcc_basic_test(void **state);
void bcc_non_branch_test(void **state);
void bcc_negative_offset_test(void **state);
void bcc_negative_offset_non_branch_test(void **state);
void lbcc_basic_test(void **state);
void lbcc_non_branch_test(void **state);
void lbcc_negative_offset_test(void **state);
void lbcc_negative_offset_non_branch_test(void **state);
void bcs_basic_test(void **state);
void bcs_non_branch_test(void **state);
void lbcs_basic_test(void **state);
void lbcs_non_branch_test(void **state);
void beq_basic_test(void **state);
void beq_non_branch_test(void **state);
void lbeq_basic_test(void **state);
void lbeq_non_branch_test(void **state);
void bge_basic_test(void **state);
void bge_non_branch_test(void **state);
void lbge_basic_test(void **state);
void lbge_non_branch_test(void **state);
void bgt_basic_test(void **state);
void bgt_non_branch_test(void **state);
void lbgt_basic_test(void **state);
void lbgt_non_branch_test(void **state);
void bhi_basic_test(void **state);
void bhi_non_branch_test(void **state);
void ble_basic_test(void **state);
void ble_non_branch_test(void **state);
void lble_basic_test(void **state);
void lble_non_branch_test(void **state);
void bls_basic_test(void **state);
void bls_non_branch_test(void **state);
void lbls_basic_test(void **state);
void lbls_non_branch_test(void **state);
void blt_basic_test(void **state);
void blt_non_branch_test(void **state);
void lblt_basic_test(void **state);
void lblt_non_branch_test(void **state);
void bmi_basic_test(void **state);
void bmi_non_branch_test(void **state);
void lbmi_basic_test(void **state);
void lbmi_non_branch_test(void **state);
void bne_basic_test(void **state);
void bne_non_branch_test(void **state);
void lbne_basic_test(void **state);
void lbne_non_branch_test(void **state);
void bpl_basic_test(void **state);
void bpl_non_branch_test(void **state);
void lbpl_basic_test(void **state);
void lbpl_non_branch_test(void **state);
void bra_basic_test(void **state);
void lbra_basic_test(void **state);
void brn_basic_test(void **state);
void lbrn_basic_test(void **state);
void bsr_basic_test(void **state);
void lbsr_basic_test(void **state);
void bvc_basic_test(void **state);
void bvc_non_branch_test(void **state);
void lbvc_basic_test(void **state);
void lbvc_non_branch_test(void **state);
void bvs_basic_test(void **state);
void bvs_non_branch_test(void **state);
void lbvs_basic_test(void **state);
void lbvs_non_branch_test(void **state);

/* Program test stuff including DECB support */
char* get_test_program_path(const char* prog_name);
void perform_memory_checks(struct test_check* checks, size_t len);
void init_from_decb_memory_test(void **state);
void init_from_decb_memory_run_cycles_test(void **state);
void init_from_decb_memory_invalid_preamble_test(void **state);
void init_from_decb_memory_invalid_preamble_2_test(void **state);
void init_from_decb_memory_invalid_postamble_test(void **state);
void init_from_decb_memory_invalid_preamble_length_test(void **state);
void init_from_decb_memory_invalid_payload_size_test(void **state);
void init_from_decb_memory_invalid_postamble_too_short_test(void **state);
void init_from_decb_memory_invalid_postamble_too_long_test(void **state);
void init_from_decb_memory_invalid_postamble_byte_wrong_test(void **state);
void init_from_decb_file_basic_test(void **state);
void init_from_decb_file_basic_run_cycles_test(void **state);
void init_from_decb_file_error_test(void **state);

/* Program tests */
void program_8_bit_addition_test(void **state);
void program_shift_left_1_bit_test(void **state);
void program_mask_off_most_significant_four_bits_test(void **state);
void program_find_larger_of_two_numbers_test(void **state);
void program_table_of_squares_test(void **state);
