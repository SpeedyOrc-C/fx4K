#include "fxlib.h"
#include "MonochromeLib.h"
#include "game/menu.h"
#include "stdio.h"
#include "FxNeue.h"

typedef struct
{
    unsigned char R128CNT;
    unsigned char gap1;
    unsigned char RSECCNT;
    unsigned char gap2;
    unsigned char RMINCNT;
    unsigned char gap3;
    unsigned char RHOURCNT;
    unsigned char gap4;
    unsigned char RWEEKCNT;
    unsigned char gap5;
    unsigned char RDAYCNT;
    unsigned char gap6;
    unsigned char RMONCNT;
    unsigned char gap7;
    unsigned short RYEARCNT;
} TRTC;

struct Temps
{
    int year;
    int mons;
    int days;
    int hour;
    int mins;
    int secs;
    int mils;
};

int BCD2int(unsigned char *BCD, int bytes)
{
    int i, n;
    for (i = 0, n = 0; i < bytes; i++)
    {
        n += (*(BCD + i) & 240) >> 4;
        n *= 10;
        n += *(BCD + i) & 15;
        if (i != bytes - 1)
            n *= 10;
    }
    return n;
}

void RTC2Time(unsigned char *rtc, struct Temps *ptemps)
{
    ptemps->year = BCD2int(rtc, 2);
    ptemps->mons = BCD2int(rtc + 2, 1);
    ptemps->days = BCD2int(rtc + 3, 1);
    ptemps->hour = BCD2int(rtc + 4, 1);
    ptemps->mins = BCD2int(rtc + 5, 1);
    ptemps->secs = BCD2int(rtc + 6, 1);
    ptemps->mils = BCD2int(rtc + 7, 1);
}

void RTC_GetDateTime(struct Temps *time)
{
    unsigned char rtc[9];
    volatile TRTC *pRTC;
    int i;

    switch (GetMPU())
    {
    case 3:
    case 4:
        pRTC = (TRTC *)0xA413FEC0;
        break;
    case 2:
    case 1:
    case 5:
        pRTC = (TRTC *)0xFFFFFEC0;
        break; // mpu7705 ?
    default:
        pRTC = 0;
        break;
    }

    if (pRTC)
    {
        rtc[0] = (*pRTC).RYEARCNT >> 8;
        rtc[1] = (*pRTC).RYEARCNT & 0x00FF;
        rtc[2] = (*pRTC).RMONCNT;
        rtc[3] = (*pRTC).RDAYCNT;
        rtc[4] = (*pRTC).RHOURCNT;
        rtc[5] = (*pRTC).RMINCNT;
        rtc[6] = (*pRTC).RSECCNT;
        rtc[7] = (*pRTC).R128CNT;
        rtc[8] = 0;
        RTC2Time(rtc, time);
    }
    else
        for (i = 0; i < 8; i++)
            rtc[i] = 0;
}

int GetMPU()
{
    int mpu;
    unsigned char savevalue;
    mpu = 0;
    savevalue = *(unsigned short *)0xA4000114;
    *(unsigned short *)0xA4000114 = 0xFFFF;

    switch (*(unsigned short *)0xA4000114)
    {
    case 0x0FFF:
        mpu = 1;
        break;
    case 0x00FF:
        mpu = 2;
        break;
    default:
        switch (*(unsigned int *)0xFF000030 & 0xFFFFFF00)
        {
        case 0x10300B00:
            switch (*(unsigned int *)0xFF000044 & 0xFFFFFFF0)
            {
            case 0x00002C00:
                mpu = 3;
                break;
            case 0x00002200:
                mpu = 4;
                break;
            }
            break;
        }
        break;
    };
    *(unsigned short *)0xA4000114 = savevalue;

    return mpu;
}

int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key;
    // struct Temps time;
    // unsigned char s[20];
    // int minmils = 999;
    // int maxmils = 0;

    // while (1)
    // {
    //     if (time.secs == 0)
    //         Bdisp_AllClr_DD();
    //     RTC_GetDateTime(&time);
    //     if (time.mils < minmils) minmils = time.mils;
    //     if (time.mils > maxmils) maxmils = time.mils;

    //     sprintf(s, "%03i - %03i", minmils, maxmils);
    //     locate(1, 1);
    //     Print(s);
    //     Bdisp_PutDisp_DD();
    // }

    menu();

    return 1;
}

//**************  Please do not change the following source.  ****************
#pragma section _BR_Size
unsigned long BR_Size;
#pragma section
#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section
