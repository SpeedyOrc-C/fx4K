#include "../MonochromeLib.h"
#include "../../fxlib.h"

#include "select.h"

char select(unsigned char *file_path)
{
    unsigned int key;
    unsigned char i, file_path_ptr, lowercase;

    lowercase = 0;

    file_path[0] = '\\';
    file_path[1] = '\\';
    file_path[2] = 'f';
    file_path[3] = 'l';
    file_path[4] = 's';
    file_path[5] = '0';
    file_path[6] = '\\';

    for (i = 7; i < 32; i++)
        file_path[i] = 0;
    file_path_ptr = 7;

    while (1)
    {
        ML_clear_vram();

        locate(1, 1);
        Print((unsigned char *)"Open chart...");

        locate(1, 3);
        Print(file_path);
        locate(8, 3);
        Print((unsigned char*)"                     ");
        locate(1, 4);
        Print(&file_path[7]);

        locate(1, 8);
        Print((unsigned char*)"FLS SD");

        locate(21, 8);
        if (lowercase)
            Print((unsigned char *)"a");
        else
            Print((unsigned char *)"A");

        GetKey(&key);

        if (file_path_ptr < 31)
        {
            if (KEY_CHAR_A <= key && key <= KEY_CHAR_Z) // Letters
            {
                if (lowercase)
                    file_path[file_path_ptr] = (unsigned char)key + 32;
                else
                    file_path[file_path_ptr] = (unsigned char)key;
                file_path_ptr++;
            }
            if (KEY_CHAR_0 <= key && key <= KEY_CHAR_9) // Numbers
            {
                file_path[file_path_ptr] = key;
                file_path_ptr++;
            }
            if (key == KEY_CHAR_DP) // Special characters
            {
                file_path[file_path_ptr] = '.';
                file_path_ptr++;
            }
            if (key == KEY_CHAR_MINUS)
            {
                file_path[file_path_ptr] = '-';
                file_path_ptr++;
            }
            if (key == KEY_CHAR_PMINUS)
            {
                file_path[file_path_ptr] = '_';
                file_path_ptr++;
            }
            if (key == KEY_CHAR_DIV)
            {
                file_path[file_path_ptr] = '\\';
                file_path_ptr++;
            }
        }
        if (key == KEY_CTRL_DEL && file_path_ptr > 7) // Backspace
        {
            file_path_ptr--;
            file_path[file_path_ptr] = 0;
        }
        if (key == KEY_CTRL_F1) // Change directory to flash
        {
            file_path[0] = '\\';
            file_path[1] = '\\';
            file_path[2] = 'f';
            file_path[3] = 'l';
            file_path[4] = 's';
            file_path[5] = '0';
            file_path[6] = '\\';
        }
        if (key == KEY_CTRL_F2) // Change directory to SD card
        {
            file_path[0] = '\\';
            file_path[1] = '\\';
            file_path[2] = 'c';
            file_path[3] = 'r';
            file_path[4] = 'd';
            file_path[5] = '0';
            file_path[6] = '\\';
        }
        if (key == KEY_CTRL_F6) // Switch case
            lowercase ^= 1;
        if (key == KEY_CTRL_EXE)
            return 1;
        if (key == KEY_CTRL_EXIT)
            return 0;
    }
}
