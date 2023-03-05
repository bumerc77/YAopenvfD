#include "openvfd.h"
#include "glyphs.h"
#include <fcntl.h>
#define OPENVFD_NAME    "openvfd"
#define OPENVFD_DEV     "/dev/" OPENVFD_NAME

static int openvfd_fd = -1;
static unsigned short openvfd_glyphs_lookup_id = 0;

const uint8_t openvfd_led_dots[OPENVFD_LED_DOT_MAX] = {
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000
};

struct openvfd_write_sequence {
    uint8_t dots;
    uint8_t pad_0;
    uint8_t char_1;
    uint8_t pad_1;
    uint8_t char_2;
    uint8_t pad_2;
    uint8_t char_3;
    uint8_t pad_3;
    uint8_t char_4;
    uint8_t pad_4;
};

static inline void openvfd_clean_write_sequance(struct openvfd_write_sequence *const sequence) {
    sequence->char_1 = '\0';
    sequence->char_2 = '\0';
    sequence->char_3 = '\0';
    sequence->char_4 = '\0';
}

int openvfd_prepare() {
    if ((openvfd_fd = open(OPENVFD_DEV, O_RDWR)) < 0) {
        pr_error_with_errno("Failed to open '"OPENVFD_DEV"' for I/O operation");
        return 1;
    }
    struct openvfd_display display = {0};
    if (ioctl(openvfd_fd, OPENVFD_IOC_GDISPLAY_TYPE, &display)) {
        pr_error_with_errno("Failed to read display type");
        return 2;
    } 
    switch(display.type) {
        case OPENVFD_DISPLAY_TYPE_5D_7S_T95:
            openvfd_glyphs_lookup_id = 0;
            break;
        case OPENVFD_DISPLAY_TYPE_5D_7S_G9SX:
            openvfd_glyphs_lookup_id = 2;
            break;
        case OPENVFD_DISPLAY_TYPE_4D_7S_FREESATGTC:
            openvfd_glyphs_lookup_id = 3;
            break;
        case OPENVFD_DISPLAY_TYPE_5D_7S_TAP1:
            openvfd_glyphs_lookup_id = 4;
            break;
        default:
            openvfd_glyphs_lookup_id = 1;
            break;
    }
    pr_debug("Using glyphs lookup id %d\n", openvfd_glyphs_lookup_id);
    return 0;
}

#define OPENVFD_EXPAND_BIT_SEQUANCE(X) \


void openvfd_write_report(char const report[5]) {
    static struct openvfd_write_sequence sequence = {0};
    // openvfd_clean_write_sequance(&sequence);
    sequence.char_1 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[0]];
    sequence.char_2 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[1]];
    sequence.char_3 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[2]];
    sequence.char_4 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[3]];
    pr_debug(
        "Characters: "
            "1: 0b"FORMAT_BIT_SEQUANCE", "
            "2: 0b"FORMAT_BIT_SEQUANCE", "
            "3: 0b"FORMAT_BIT_SEQUANCE", "
            "4: 0b"FORMAT_BIT_SEQUANCE"\n", 
            expand_bit_sequance(sequence.char_1),
            expand_bit_sequance(sequence.char_2),
            expand_bit_sequance(sequence.char_3),
            expand_bit_sequance(sequence.char_4));
}