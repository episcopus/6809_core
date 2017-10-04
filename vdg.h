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

#define VDG_BLACK     0x000000FF
#define VDG_WHITE     0xFFFFFFFF
#define VDG_GREEN     0x0BFF02FF
#define VDG_AI_INV    0x004100FF

#define VDG_BUF_X 256
#define VDG_BUF_Y 192
#define ACTIVE_BUF_OFFSET 25
/* SCR_BUF_X == 306 */
#define SCR_BUF_X (VDG_BUF_X + ACTIVE_BUF_OFFSET + ACTIVE_BUF_OFFSET)
/* SCR_BUF_Y == 242 */
#define SCR_BUF_Y (VDG_BUF_Y + ACTIVE_BUF_OFFSET + ACTIVE_BUF_OFFSET)
/* SCR_BUF_Y * SCR_BUF_X == 74,052 words == 296,208 bytes */

#define VDG_R_BYTE(color_word)    ((color_word & 0xFF000000) >> 24)
#define VDG_G_BYTE(color_word)    ((color_word & 0x00FF0000) >> 16)
#define VDG_B_BYTE(color_word)    ((color_word & 0x0000FF00) >> 8)
#define VDG_A_BYTE(color_word)    (color_word & 0x000000FF)

struct vdg_state_struct {
    /* SCR_BUF_Y * SCR_BUF_X bytes in sRGBA format */
    uint8* video_buf;
};

uint8 get_vdg_mode_from_sam();
uint16 get_video_starting_address_from_sam();
enum vdg_mode get_vdg_mode();
void vdg_init();
void vdg_destroy();
void vdg_update();
void vdg_buf_set_pixel(uint16 x, uint16 y, uint32 color);
void vdg_clear_buffer(uint32 border_color);
void vdg_update_ai_sg4_mode(uint16 buf_addr);
void vdg_update_by_mode(enum vdg_mode mode, uint16 buf_addr, uint32 border_color);
uint32 vdg_get_bg_color(enum vdg_mode mode);
