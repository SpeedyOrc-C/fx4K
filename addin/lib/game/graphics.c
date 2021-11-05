#include "../MonochromeLib.h"
#include "../../fxlib.h"

#include "play.h"

void draw_key_press_effect(unsigned char *key_state)
{
    if (key_state[0])
        ML_rectangle(
            1, 64 - COLUMN_HEIGHT,
            COLUMN_WIDTH, 62,
            2, ML_XOR, ML_WHITE);
    if (key_state[1])
        ML_rectangle(
            1 + COLUMN_WIDTH, 64 - COLUMN_HEIGHT,
            COLUMN_WIDTH * 2, 62,
            2, ML_XOR, ML_WHITE);
    if (key_state[2])
        ML_rectangle(
            1 + COLUMN_WIDTH * 2, 64 - COLUMN_HEIGHT,
            COLUMN_WIDTH * 3, 62,
            2, ML_XOR, ML_WHITE);
    if (key_state[3])
        ML_rectangle(
            1 + COLUMN_WIDTH * 3, 64 - COLUMN_HEIGHT,
            COLUMN_WIDTH * 4, 62,
            2, ML_XOR, ML_WHITE);
}


void draw_frame()
{
    unsigned char i;
    for (i = 0; i <= 62; i += 2)
    {
        ML_pixel(0, i, ML_BLACK);
        ML_pixel(COLUMN_WIDTH, i, ML_BLACK);
        ML_pixel(COLUMN_WIDTH * 2, i, ML_BLACK);
        ML_pixel(COLUMN_WIDTH * 3, i, ML_BLACK);
        ML_pixel(COLUMN_WIDTH * 4, i, ML_BLACK);
    }
    ML_line(0, 63 - COLUMN_HEIGHT, COLUMN_WIDTH * 4, 63 - COLUMN_HEIGHT, ML_BLACK);
    ML_line(0, 63, COLUMN_WIDTH * 4, 63, ML_BLACK);
}


void draw_progress_bar(short time, short song_length)
{
    ML_rectangle(
        124, 0,
        127, 63,
        1, ML_BLACK, ML_WHITE);
    ML_rectangle(
        125, 1,
        127, 1+61*time/song_length,
        1, ML_BLACK, ML_BLACK);
}
