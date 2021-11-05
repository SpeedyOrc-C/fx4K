#include "key_test.h"
#include "../MonochromeLib.h"
#include "../../fxlib.h"

void key_test()
{
    
    char key1, key2, key3, key4, key_exit;

    while (1)
    {
        ML_clear_vram();

        locate(1, 1);
        Print((unsigned char*)"----- Key Test -----");
        locate(1, 2);
        Print((unsigned char*)"Checklist:");
        locate(1, 3);
        Print((unsigned char*)" * Key latency");
        locate(1, 4);
        Print((unsigned char*)" * Multitouch");
        locate(1, 5);
        Print((unsigned char*)" * Key is not broken!");

        if (IsKeyDown(KEY_CHAR_7)) key1 = 1;
        else key1 = 0;
        if (IsKeyDown(KEY_CHAR_8)) key2 = 1;
        else key2 = 0;
        if (IsKeyDown(KEY_CTRL_DEL)) key3 = 1;
        else key3 = 0;
        if (IsKeyDown(KEY_CTRL_AC)) key4 = 1;
        else key4 = 0;
        if (IsKeyDown(KEY_CTRL_EXIT)) key_exit = 1;
        else key_exit = 0;

        locate(2, 8);
        if (key1) PrintRev((unsigned char*)"[01]");
        else Print((unsigned char*)"[01]");

        locate(7, 8);
        if (key2) PrintRev((unsigned char*)"[02]");
        else Print((unsigned char*)"[02]");

        locate(12, 8);
        if (key3) PrintRev((unsigned char*)"[03]");
        else Print((unsigned char*)"[03]");

        locate(17, 8);
        if (key4) PrintRev((unsigned char*)"[04]");
        else Print((unsigned char*)"[04]");

        ML_display_vram();
        if (key_exit) break;
    }

}

