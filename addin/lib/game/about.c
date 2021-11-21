#include "about.h"
#include "../MonochromeLib.h"
#include "../../fxlib.h"

void about()
{
    unsigned int key;
    unsigned char page;
    
    page = 0;
    while (1)
    {
        ML_clear_vram();
        if (page == 0)
        {
            locate(1, 1);
            Print((unsigned char*)"------- Help --------");
            locate(1, 3);
            Print((unsigned char*)"Press these keys:");
            locate(1, 4);
            Print((unsigned char*)"[7]  [8]  [DEL] [AC]");
            locate(1, 6);
            Print((unsigned char*)"[EXIT] Pause");
            locate(19, 8);
            Print((unsigned char*)"1/2");
        }
        if (page == 1)
        {
            locate(1, 1);
            Print((unsigned char*)"------- About -------");
            locate(5, 3);
            Print((unsigned char*)"Chen Zhanming");
            locate(3, 5);
            Print((unsigned char*)"1670732206@qq.com");
            locate(7, 7);
            Print((unsigned char*)"fx4K v0.3");
            locate(19, 8);
            Print((unsigned char*)"2/2");
        }

        GetKey(&key);
        if (key == KEY_CTRL_RIGHT && page < 1) page++;
        if (key == KEY_CTRL_LEFT && page > 0) page--;
        if (key == KEY_CTRL_EXIT) break;
    }

}
