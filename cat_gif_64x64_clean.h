#ifndef CAT_GIF_64X64_CLEAN_H
#define CAT_GIF_64X64_CLEAN_H

/* 64x64 clean black/white cat animation for KS0108 LCD12864
 * - Pure threshold, no dithering
 * - 8 frames
 * - Page-major data: 8 pages x 64 columns = 512 bytes per frame
 * - bit0 is the top pixel inside each page
 * - Pixel ON data is 1
 */

#define CAT64_WIDTH            64
#define CAT64_HEIGHT           64
#define CAT64_PAGES            8
#define CAT64_FRAME_BYTES      512
#define CAT64_FRAME_COUNT      8
#define CAT64_X_OFFSET         32   /* centered on 128x64 screen */

extern const unsigned char code g_cat64_clean[CAT64_FRAME_COUNT][CAT64_FRAME_BYTES];

#endif
