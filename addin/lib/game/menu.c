#include "../MonochromeLib.h"
#include "../../fxlib.h"

#include "select.h"
#include "play.h"
#include "key_test.h"
#include "about.h"

void menu()
{
    unsigned int key;
    unsigned char selection;
    unsigned char file_path[32];

    selection = 0;
    while (1)
    {
        // Main menu

        ML_clear_vram();

        locate(3, 3);
        Print((unsigned char*)"fx4K");
        locate(2, 5);
        Print((unsigned char*)"In 2021!");

        locate(11, 2);
        if (selection == 0)
            Print((unsigned char*)"> PLAY");
        else
            Print((unsigned char*)"  PLAY");

        locate(11, 4);
        if (selection == 1)
            Print((unsigned char*)"> KEY TEST");
        else
            Print((unsigned char*)"  KEY TEST");
        
        locate(11, 6);
        if (selection == 2)
            Print((unsigned char*)"> ABOUT");
        else
            Print((unsigned char*)"  ABOUT");

        ML_display_vram();

        // Move cursor

        GetKey(&key);
        if (key == KEY_CTRL_UP && selection > 0) selection--;
        if (key == KEY_CTRL_DOWN && selection < 2) selection++;
        if (key == KEY_CTRL_EXE)
        {
            if (selection == 0)
            {
                if (select(file_path))
                    play(file_path);
            }
            if (selection == 1)
                key_test();
            if (selection == 2)
                about();
        }
    }


}