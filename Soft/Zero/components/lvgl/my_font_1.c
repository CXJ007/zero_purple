/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MY_FONT_1
#define MY_FONT_1 1
#endif

#if MY_FONT_1

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+002F "/" */
    0x2, 0x6, 0xc, 0x8, 0x18, 0x10, 0x0, 0x20,
    0x60, 0x40, 0xc0, 0x80, 0x0,

    /* U+0030 "0" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x40, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0031 "1" */
    0x7d, 0xf0,

    /* U+0032 "2" */
    0x7c, 0x4, 0x8, 0x10, 0x20, 0x5f, 0x40, 0x81,
    0x2, 0x4, 0x7, 0xc0,

    /* U+0033 "3" */
    0x7c, 0x4, 0x8, 0x10, 0x20, 0x7f, 0x1, 0x2,
    0x4, 0x8, 0x17, 0xc0,

    /* U+0034 "4" */
    0x1, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x1, 0x2,
    0x4, 0x8, 0x10, 0x0,

    /* U+0035 "5" */
    0x7d, 0x2, 0x4, 0x8, 0x10, 0x1f, 0x1, 0x2,
    0x4, 0x8, 0x17, 0xc0,

    /* U+0036 "6" */
    0x7d, 0x2, 0x4, 0x8, 0x10, 0x1f, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0037 "7" */
    0x7c, 0x4, 0x8, 0x10, 0x20, 0x40, 0x1, 0x2,
    0x4, 0x8, 0x10,

    /* U+0038 "8" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0039 "9" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x1, 0x2,
    0x4, 0x8, 0x17, 0xc0,

    /* U+0041 "A" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x41, 0x83,
    0x6, 0xc, 0x10, 0x0,

    /* U+0042 "B" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0043 "C" */
    0x7f, 0x2, 0x4, 0x8, 0x10, 0x0, 0x40, 0x81,
    0x2, 0x4, 0x7, 0xe0,

    /* U+0044 "D" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x40, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0045 "E" */
    0x7f, 0x2, 0x4, 0x8, 0x10, 0x1f, 0xc0, 0x81,
    0x2, 0x4, 0x7, 0xe0,

    /* U+0046 "F" */
    0x7f, 0x2, 0x4, 0x8, 0x10, 0x1f, 0xc0, 0x81,
    0x2, 0x4, 0x0, 0x0,

    /* U+0047 "G" */
    0x7d, 0x2, 0x4, 0x8, 0x10, 0x3, 0xc1, 0x83,
    0x6, 0xf, 0xf7, 0xc0,

    /* U+0048 "H" */
    0x1, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x41, 0x83,
    0x6, 0xc, 0x10, 0x0,

    /* U+0049 "I" */
    0xfd, 0xf8,

    /* U+004A "J" */
    0x0, 0x4, 0x8, 0x10, 0x20, 0x40, 0x1, 0x3,
    0x6, 0xc, 0x17, 0xc0,

    /* U+004B "K" */
    0x7, 0x1a, 0x65, 0x8e, 0x18, 0x1f, 0x41, 0x83,
    0x6, 0xc, 0x10, 0x0,

    /* U+004C "L" */
    0x1, 0x2, 0x4, 0x8, 0x10, 0x0, 0x40, 0x81,
    0x2, 0x4, 0x7, 0xe0,

    /* U+004D "M" */
    0x7d, 0x26, 0x4c, 0x99, 0x32, 0x40, 0x41, 0x83,
    0x6, 0xc, 0x10, 0x0,

    /* U+004E "N" */
    0x1, 0x87, 0x8d, 0x99, 0xb1, 0xc0, 0x41, 0x83,
    0x6, 0xc, 0x18, 0x20,

    /* U+004F "O" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x40, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0050 "P" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x40, 0x81,
    0x2, 0x4, 0x0, 0x0,

    /* U+0051 "Q" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x40, 0x41, 0x83,
    0x6, 0xc, 0x27, 0xa0,

    /* U+0052 "R" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x50, 0xb1,
    0x32, 0x34, 0x30, 0x20,

    /* U+0053 "S" */
    0x7f, 0x2, 0x4, 0x8, 0x10, 0x1f, 0x1, 0x2,
    0x4, 0x8, 0x1f, 0xc0,

    /* U+0054 "T" */
    0x7c, 0x20, 0x40, 0x81, 0x2, 0x0, 0x8, 0x10,
    0x20, 0x40, 0x81, 0x0,

    /* U+0055 "U" */
    0x1, 0x6, 0xc, 0x18, 0x30, 0x40, 0x41, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+0056 "V" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x40, 0xc3, 0x8d,
    0x32, 0xc7, 0x8, 0x0,

    /* U+0057 "W" */
    0x1, 0x6, 0xc, 0x18, 0x30, 0x40, 0x49, 0x93,
    0x26, 0x4c, 0x97, 0xc0,

    /* U+0058 "X" */
    0x81, 0xc3, 0x42, 0x66, 0x3c, 0x18, 0x0, 0x18,
    0x3c, 0x66, 0x42, 0xc3, 0x81,

    /* U+0059 "Y" */
    0x80, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0x7e, 0x1,
    0x1, 0x1, 0x1, 0x1, 0x7e,

    /* U+005A "Z" */
    0x7f, 0x3, 0x2, 0x6, 0xc, 0x8, 0x0, 0x30,
    0x60, 0x60, 0xc0, 0x80, 0xfe
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 76, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 147, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 14, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 83, .box_w = 1, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 52, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 99, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 123, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 159, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 83, .box_w = 1, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 221, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 293, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 305, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 365, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 157, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 389, .adv_w = 157, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 402, .adv_w = 157, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 157, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 47, .range_length = 11, .glyph_id_start = 2,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 13,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t my_font_1 = {
#else
lv_font_t my_font_1 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 13,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if MY_FONT_1*/

