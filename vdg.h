enum vdg_mode {
    VDG_AI,
    VDG_SG4,
    VDG_SG6,
    VDG_SG8,
    VDG_SG12,
    VDG_SG24,
    VDG_G1C,
    VDG_G1R,
    VDG_G2C,
    VDG_G2R,
    VDG_G3C,
    VDG_G3R,
    VDG_G6C,
    VDG_G6R
};

uint8 get_vdg_mode_from_sam();
uint16 get_video_starting_address_from_sam();
enum vdg_mode get_vdg_mode();
