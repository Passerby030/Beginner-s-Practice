#include <REGX52.H>
#include "ks0108_lcd.h"
#include "cat_gif_64x64_clean.h"

#define CAT_GIF_INVERT  1

/* 只刷新中间 64x64 区域，并且仅写“与上一帧不同”的字节。
 * 这样比整屏 128x64 全刷更快，Proteus 里逐行刷新的感觉会减轻很多。
 */

static void LCD_WritePageRun(unsigned char page, unsigned char x_start, const unsigned char code *p, unsigned char len)
{
    unsigned char i;
    GLCD_SetColumn(x_start);
    GLCD_SetPage(page);
    for(i = 0; i < len; i++)
    {
#if CAT_GIF_INVERT
        GLCD_WriteData((unsigned char)(~p[i]));
#else
        GLCD_WriteData(p[i]);
#endif
    }
}

static void Cat64_DrawFirstFrame(unsigned char frame_id)
{
    unsigned char page;
    const unsigned char code *p;

    if(frame_id >= CAT64_FRAME_COUNT)
    {
        frame_id = 0;
    }

    p = g_cat64_clean[frame_id];

    for(page = 0; page < CAT64_PAGES; page++)
    {
        /* 左半片：屏幕列 32~63，对应图像列 0~31 */
        LCD_WritePageRun(page, 32, p + page * 64, 32);

        /* 右半片：屏幕列 64~95，对应图像列 32~63 */
        LCD_WritePageRun(page, 64, p + page * 64 + 32, 32);
    }
}

static void Cat64_DrawDiffFrame(unsigned char prev_id, unsigned char cur_id)
{
    unsigned char page;
    unsigned char col;
    unsigned char run_start;
    unsigned char in_run;
    const unsigned char code *p_prev;
    const unsigned char code *p_cur;

    if(prev_id >= CAT64_FRAME_COUNT) prev_id = 0;
    if(cur_id  >= CAT64_FRAME_COUNT) cur_id  = 0;

    p_prev = g_cat64_clean[prev_id];
    p_cur  = g_cat64_clean[cur_id];

    for(page = 0; page < CAT64_PAGES; page++)
    {
        /* 先处理图像左半 0~31 -> 屏幕 32~63 */
        in_run = 0;
        run_start = 0;
        for(col = 0; col < 32; col++)
        {
            if(p_prev[page * 64 + col] != p_cur[page * 64 + col])
            {
                if(!in_run)
                {
                    in_run = 1;
                    run_start = col;
                }
            }
            else
            {
                if(in_run)
                {
                    LCD_WritePageRun(page,
                                     (unsigned char)(32 + run_start),
                                     p_cur + page * 64 + run_start,
                                     (unsigned char)(col - run_start));
                    in_run = 0;
                }
            }
        }
        if(in_run)
        {
            LCD_WritePageRun(page,
                             (unsigned char)(32 + run_start),
                             p_cur + page * 64 + run_start,
                             (unsigned char)(32 - run_start));
        }

        /* 再处理图像右半 32~63 -> 屏幕 64~95 */
        in_run = 0;
        run_start = 32;
        for(col = 32; col < 64; col++)
        {
            if(p_prev[page * 64 + col] != p_cur[page * 64 + col])
            {
                if(!in_run)
                {
                    in_run = 1;
                    run_start = col;
                }
            }
            else
            {
                if(in_run)
                {
                    LCD_WritePageRun(page,
                                     (unsigned char)(64 + (run_start - 32)),
                                     p_cur + page * 64 + run_start,
                                     (unsigned char)(col - run_start));
                    in_run = 0;
                }
            }
        }
        if(in_run)
        {
            LCD_WritePageRun(page,
                             (unsigned char)(64 + (run_start - 32)),
                             p_cur + page * 64 + run_start,
                             (unsigned char)(64 - run_start));
        }
    }
}

void main(void)
{
    unsigned char prev_id;
    unsigned char cur_id;

    GLCD_Init();
    GLCD_Clear();

    prev_id = 0;
    Cat64_DrawFirstFrame(prev_id);

    while(1)
    {
        for(cur_id = 1; cur_id < CAT64_FRAME_COUNT; cur_id++)
        {
            Cat64_DrawDiffFrame(prev_id, cur_id);
            prev_id = cur_id;
            DelayMs(70);
        }

        Cat64_DrawDiffFrame(prev_id, 0);
        prev_id = 0;
        DelayMs(70);
    }
}
