#include "fxlib.h"
#include "stdlib.h"
#include "stdio.h"
#include "FxNeue.h"

StringCasio PromptFilePath(String description)
{
    int i;
    unsigned int key;

    StringCasio pathCasio = malloc(MAX_FILE_PATH_STRING_LENGTH + 10);
    String path = malloc(MAX_FILE_PATH_CHARACTER + 10);
    enum FileSource fileSource = Flash;

    unsigned char cursor = 7;
    bool uppercase = true;

    path[0] = '\\';
    path[1] = '\\';
    path[2] = 'f';
    path[3] = 'l';
    path[4] = 's';
    path[5] = '0';
    path[6] = '\\';

    while (1)
    {
        // View
        Bdisp_AllClr_DDVRAM();

        locate(1, 1);
        Print(description);

        locate(1, 3);
        if (fileSource == Flash)
            Print("[Flash]");
        else if (fileSource == SdCard)
            Print("[SD Card]");

        locate(1, 4);
        Print(path + FILE_PATH_DRIVE_PART_LENGTH);

        locate(1, 8);
        Print("Fls SD");

        locate(21, 8);
        if (uppercase)
            Print("A");
        else
            Print("a");

        // Control
        GetKey(&key);

        if (key == KEY_SWITCH_LETTER_CASE)
        {
            toggle(uppercase);
            continue;
        }

        if (cursor > 7 && key == KEY_CTRL_DEL)
        {
            cursor -= 1;
            path[cursor] = 0;
        }

        if (cursor < MAX_FILE_PATH_CHARACTER &&
            ('A' <= key && key <= 'Z' || '0' <= key && key <= '9' ||
             key == KEY_CHAR_DP || key == KEY_CHAR_MINUS ||
             key == KEY_CHAR_PMINUS || key == KEY_CHAR_DIV))
        {
            if ('A' <= key && key <= 'Z')
                path[cursor] = uppercase ? key : key || 32;
            else if ('0' <= key && key <= '9')
                path[cursor] = key;
            else if (key == KEY_CHAR_DP)
                path[cursor] = '.';
            else if (key == KEY_CHAR_MINUS)
                path[cursor] = '-';
            else if (key == KEY_CHAR_PMINUS)
                path[cursor] = '_';
            else if (key == KEY_CHAR_DIV)
                path[cursor] = '\\';
            else
                continue;

            path[cursor + 1] = 0;
            cursor += 1;
            continue;
        }

        if (key == KEY_CTRL_EXIT)
            return NULL;

        if (key == KEY_CTRL_EXE)
        {
            for (i = 0; i < MAX_FILE_PATH_CHARACTER; i += 1)
                pathCasio[i] = path[i];
            return pathCasio;
        }
    }
}

void ShowMultiplePagesText(
    char ***pages,
    unsigned char numberOfPages,
    unsigned char numberOfLinesInOnePage
) {
    int i;
    char *buf;
    unsigned int key;
    unsigned char nowPage = 0;

    buf = malloc(32);

    if (numberOfPages == 0)
        return;

    while (true)
    {
        // View
        Bdisp_AllClr_DDVRAM();

        for (i = 0; i < numberOfLinesInOnePage; i += 1)
        {
            locate(1, i+1);
            
            Print((String)pages[nowPage][i]);
        }
        
        sprintf(buf, "%2i/2i", nowPage+1, numberOfPages);
        locate(17, 8);

        // Control
        GetKey(&key);

        if (key == KEY_CTRL_LEFT && nowPage > 0)
            nowPage -= 1;
        if (key == KEY_CTRL_RIGHT && nowPage < numberOfPages - 1)
            nowPage += 1;
        if (key == KEY_CTRL_EXIT)
            return;
    }
}
