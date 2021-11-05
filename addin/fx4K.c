// Copyright (c) 2006 CASIO COMPUTER CO., LTD.
#include "fxlib.h"
#include "lib/MonochromeLib.h"
#include "lib/game/menu.h"


int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key;

    menu();

    return 1;
}




//**************  Please do not change the following source.  ****************
#pragma section _BR_Size
unsigned long BR_Size;
#pragma section
#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum)
{ return INIT_ADDIN_APPLICATION(isAppli, OptionNum); }
#pragma section
