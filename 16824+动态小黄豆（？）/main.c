#include <reg51.h>
#include <intrins.h>

sbit RS  = P2^0;
sbit RW  = P2^1;
sbit E   = P2^2;
sbit CS1 = P2^3;
sbit CS2 = P2^4;

#define DATA_PORT P0

void delay_ms(unsigned int ms)
{
    unsigned int i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<123;j++);
}

void Left()  { CS1=1; CS2=0; }
void Right() { CS1=0; CS2=1; }

void Cmd(unsigned char c)
{
    RS=0; RW=0; DATA_PORT=c; E=1; _nop_(); E=0; delay_ms(1);
}

void Dat(unsigned char d)
{
    RS=1; RW=0; DATA_PORT=d; E=1; _nop_(); E=0; delay_ms(1);
}

void Init()
{
    Left();  Cmd(0x3E); Cmd(0x3F);
    Right(); Cmd(0x3E); Cmd(0x3F);
}

void Clear()
{
    unsigned char p,i;
    Left();
    for(p=0;p<8;p++){ Cmd(0xB8+p); Cmd(0x40); for(i=0;i<64;i++) Dat(0x00); }
    Right();
    for(p=0;p<8;p++){ Cmd(0xB8+p); Cmd(0x40); for(i=0;i<64;i++) Dat(0x00); }
}

// 8x8 Ð¡±íÇé
unsigned char code face_smile[8] = {0x38,0x44,0x82,0xA2,0x82,0xA2,0x44,0x38};
unsigned char code face_cry[8]   = {0x38,0x44,0x82,0xA2,0x92,0xA2,0x46,0x39};
unsigned char code face_heart[8] = {0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x18};

void Show_8x8_Center(unsigned char *pic)
{
    Left();
    Cmd(0xB8 + 3);
    Cmd(0x40 + 28);
    Dat(pic[0]); Dat(pic[1]); Dat(pic[2]); Dat(pic[3]);
    Dat(pic[4]); Dat(pic[5]); Dat(pic[6]); Dat(pic[7]);
}


void main()
{
    Init();
    Clear();
    
    while(1)
    {
        Show_8x8_Center(face_smile);
        delay_ms(150);  

        Show_8x8_Center(face_cry);
        delay_ms(150);  

        Show_8x8_Center(face_heart);
        delay_ms(150);  
    }
}