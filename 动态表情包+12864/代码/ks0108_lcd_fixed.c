#include "ks0108_lcd.h"

static unsigned char g_current_chip = 1;  /* 1 = left, 2 = right */
static unsigned char g_current_col  = 0;  /* 0~63 inside one chip */

static void DelayUs_rough(unsigned int t)
{
    while(t--)
    {
        ;
    }
}

void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 123; j++)
        {
            ;
        }
    }
}

static void GLCD_EnablePulse(void)
{
    GLCD_EN = 1;
    DelayUs_rough(4);
    GLCD_EN = 0;
    DelayUs_rough(4);
}

static void GLCD_DeselectAll(void)
{
    /* Proteus KS0108X2 model uses active-low CS */
    GLCD_CS1 = 1;
    GLCD_CS2 = 1;
}

static void GLCD_SelectChip(unsigned char chip)
{
    if(chip == 1)
    {
        GLCD_CS1 = 0;
        GLCD_CS2 = 1;
    }
    else
    {
        GLCD_CS1 = 1;
        GLCD_CS2 = 0;
    }
}

static void GLCD_SelectChipByColumn(unsigned char column)
{
    if(column < 64)
    {
        g_current_chip = 1;
        g_current_col = column;
    }
    else
    {
        g_current_chip = 2;
        g_current_col = (unsigned char)(column - 64);
    }

    GLCD_SelectChip(g_current_chip);
}

static void GLCD_WriteCommandToChip(unsigned char cmd, unsigned char chip)
{
    GLCD_RS = 0;
    GLCD_RW = 0;
    GLCD_SelectChip(chip);
    GLCD_DATA_PORT = cmd;
    GLCD_EnablePulse();
    DelayUs_rough(12);
    GLCD_DeselectAll();
}

void GLCD_Init(void)
{
    GLCD_EN = 0;
    GLCD_RS = 0;
    GLCD_RW = 0;
    GLCD_DeselectAll();

    DelayMs(20);

    GLCD_WriteCommandToChip(0x3F, 1);  /* Display ON */
    GLCD_WriteCommandToChip(0x3F, 2);
    GLCD_WriteCommandToChip(0xC0, 1);  /* Start line = 0 */
    GLCD_WriteCommandToChip(0xC0, 2);

    GLCD_Clear();
}

void GLCD_SetPage(unsigned char page)
{
    if(page > 7)
    {
        page = 7;
    }

    GLCD_WriteCommandToChip((unsigned char)(0xB8 | page), g_current_chip);
}

void GLCD_SetColumn(unsigned char column)
{
    GLCD_SelectChipByColumn(column);
    GLCD_WriteCommandToChip((unsigned char)(0x40 | g_current_col), g_current_chip);
}

void GLCD_WriteData(unsigned char dat)
{
    GLCD_RS = 1;
    GLCD_RW = 0;
    GLCD_SelectChip(g_current_chip);
    GLCD_DATA_PORT = dat;
    GLCD_EnablePulse();
    DelayUs_rough(8);
    GLCD_DeselectAll();
}

void GLCD_Clear(void)
{
    unsigned char page;
    unsigned char i;

    for(page = 0; page < 8; page++)
    {
        GLCD_SetColumn(0);
        GLCD_SetPage(page);
        for(i = 0; i < 64; i++)
        {
            GLCD_WriteData(0x00);
        }

        GLCD_SetColumn(64);
        GLCD_SetPage(page);
        for(i = 0; i < 64; i++)
        {
            GLCD_WriteData(0x00);
        }
    }
}
