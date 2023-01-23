#include "../MonochromeLib.h"
#include "fxlib.h"
#include "stdio.h"
#include <stdlib.h>

#include "play.h"
#include "graphics.h"

unsigned char buffer[32];
unsigned int key;

unsigned char wait_frame;
short column_start_ptr[4], column_now_ptr[4];
short time;
unsigned char key_state[4];
unsigned short miss_count, good_count, perfect_count, combo, max_combo;
float accuracy;
short VISIBLE = 40;

void select_speed()
{
    ML_clear_vram();

    locate(9, 2);
    Print((unsigned char *)"Speed");
    locate(7, 3);
    Print((unsigned char *)"[1] 2s");
    locate(7, 4);
    Print((unsigned char *)"[2] 1.5s");
    locate(7, 5);
    Print((unsigned char *)"[3] 1s");
    locate(7, 6);
    Print((unsigned char *)"[4] 0.75s");
    locate(7, 7);
    Print((unsigned char *)"[5] 0.5s");

    ML_display_vram();

    while (1)
    {
        GetKey(&key);
        if (KEY_CHAR_1 <= key && key <= KEY_CHAR_5)
        {
            switch (key)
            {
            case KEY_CHAR_1:
                VISIBLE = 80;
                break;
            case KEY_CHAR_2:
                VISIBLE = 60;
                break;
            case KEY_CHAR_3:
                VISIBLE = 40;
                break;
            case KEY_CHAR_4:
                VISIBLE = 30;
                break;
            case KEY_CHAR_5:
                VISIBLE = 20;
                break;
            default:
                break;
            }
            break;
        }
    }
}

unsigned int pause()
{
    unsigned int i, j;

    for (i = 0; i <= 127; i++)
        for (j = 0; j <= 63; j++)
            if ((i + j) % 2 == 1)
                ML_pixel(i, j, ML_WHITE);

    locate(4, 3);
    Print((unsigned char *)"[9]   Continue");
    locate(4, 4);
    Print((unsigned char *)"[F1]  Replay  ");
    locate(4, 5);
    Print((unsigned char *)"[F6]  Leave   ");
    ML_display_vram();

    while (1)
    {
        GetKey(&key);
        if (key == KEY_CHAR_9 || key == KEY_CTRL_F1 || key == KEY_CTRL_F6)
            return key;
    }
}

void display_result(unsigned char *chart_title)
{
    accuracy = 100 * ((float)perfect_count + (float)good_count / 2) /
               (float)(perfect_count + good_count + miss_count);

    ML_clear_vram();

    locate(1, 1);
    Print(chart_title);
    locate(1, 2);
    Print(&chart_title[21]);

    locate(1, 3);
    Print((unsigned char *)"---------------------");

    locate(1, 4);
    sprintf((char *)buffer, "ACCURACY : %.2f%%", accuracy);
    Print(buffer);
    locate(1, 5);
    sprintf((char *)buffer, "MAX COMBO: %u", max_combo);
    Print(buffer);
    locate(1, 6);
    sprintf((char *)buffer, "PERFECT  : %u", perfect_count);
    Print(buffer);
    locate(1, 7);
    sprintf((char *)buffer, "GOOD     : %u", good_count);
    Print(buffer);
    locate(1, 8);
    sprintf((char *)buffer, "MISS     : %u", miss_count);
    Print(buffer);

    ML_display_vram();

    while (1)
    {
        GetKey(&key);
        if (key == KEY_CTRL_EXE)
            break;
    }
}

void frame_timer()
{
    wait_frame = 0;
}

void note_state_machine(struct Note notes[2000])
{
    unsigned char i, score;
    unsigned short n, n2;

    for (i = 0; i < 4; i++)
    {
        if (column_now_ptr[i] != -1)
        {
            if (notes[column_now_ptr[i]].type == 'C') // Click
            {
                n = notes[column_now_ptr[i]].start_time;
                score = notes[column_now_ptr[i]].score;

                if (score == SCORE_NOT_RATED)
                {
                    if (n - PERFECT - GOOD <= time && time <= n + PERFECT + GOOD &&
                        key_state[i] == 0)
                        notes[column_now_ptr[i]].score = SCORE_RATE_READY;

                    if (n + PERFECT + GOOD < time)
                    {
                        notes[column_now_ptr[i]].score = SCORE_MISS;
                        combo = 0;
                    }
                }

                if (score == SCORE_RATE_READY)
                {
                    if ((n - PERFECT - GOOD <= time && time < n - PERFECT ||
                         n + PERFECT < time && time <= n + PERFECT + GOOD) &&
                        key_state[i] == 1)
                    {
                        notes[column_now_ptr[i]].score = SCORE_GOOD;
                        combo++;
                    }

                    if (n - PERFECT <= time && time <= n + PERFECT &&
                        key_state[i] == 1)
                    {
                        notes[column_now_ptr[i]].score = SCORE_PERFECT;
                        combo++;
                    }

                    if (n + PERFECT + GOOD < time)
                    {
                        notes[column_now_ptr[i]].score = SCORE_MISS;
                        combo = 0;
                    }
                }

                if (notes[column_now_ptr[i]].score == SCORE_MISS ||
                    notes[column_now_ptr[i]].score == SCORE_GOOD ||
                    notes[column_now_ptr[i]].score == SCORE_PERFECT)
                {
                    if (notes[column_now_ptr[i]].score == SCORE_MISS)
                        miss_count++;
                    if (notes[column_now_ptr[i]].score == SCORE_GOOD)
                        good_count++;
                    if (notes[column_now_ptr[i]].score == SCORE_PERFECT)
                        perfect_count++;
                    column_now_ptr[i] = notes[column_now_ptr[i]].next;
                }
            }

            if (notes[column_now_ptr[i]].type == 'H') // Hold
            {
                n = notes[column_now_ptr[i]].start_time;
                n2 = notes[column_now_ptr[i]].end_time;
                score = notes[column_now_ptr[i]].score;

                if (score == SCORE_NOT_RATED)
                {
                    if (n - PERFECT - GOOD <= time && time <= n + PERFECT + GOOD &&
                        key_state[i] == 1)
                        notes[column_now_ptr[i]].score = SCORE_RATE_READY;

                    if (n + PERFECT + GOOD < time)
                    {
                        notes[column_now_ptr[i]].score = SCORE_MISS;
                        combo = 0;
                    }
                }

                if (score == SCORE_RATE_READY)
                {
                    if (time < n2 - PERFECT - GOOD &&
                        key_state[i] == 0)
                    {
                        notes[column_now_ptr[i]].score = SCORE_MISS;
                        combo++;
                    }

                    if (n2 - GOOD - PERFECT <= time && key_state[i] == 1)
                    {
                        notes[column_now_ptr[i]].score = SCORE_PERFECT;
                        combo = 0;
                    }
                }

                if (notes[column_now_ptr[i]].score == SCORE_MISS ||
                    notes[column_now_ptr[i]].score == SCORE_PERFECT)
                {
                    if (notes[column_now_ptr[i]].score == SCORE_MISS)
                        miss_count++;
                    if (notes[column_now_ptr[i]].score == SCORE_PERFECT)
                        perfect_count++;
                    column_now_ptr[i] = notes[column_now_ptr[i]].next;
                }
            }
        }
    }
}

void play(unsigned char *file_path)
{
    unsigned int column;
    int i;
    int chart_file;
    FONTCHARACTER casio_file_path[32];
    unsigned char chart_title[32], chart_artist[32];
    short chart_bpm, chart_note_num, song_length;
    short visible_note_start_ptr, visible_note_end_ptr;

    struct Note notes[2000];

    for (i = 0; i < 32; i++)
        casio_file_path[i] = file_path[i];

    chart_file = Bfile_OpenFile(casio_file_path, _OPENMODE_READ_SHARE);
    if (chart_file < 0)
    {
        locate(1, 1);
        Print((unsigned char *)"Chart not found.");
        GetKey(&key);
        return;
    }

    //////////////////////////////////////
    // Read & display chart information //
    //////////////////////////////////////

    Bfile_ReadFile(chart_file, chart_title, TITLE_SIZE, TITLE_POS);
    Bfile_ReadFile(chart_file, chart_artist, ARTIST_SIZE, ARTIST_POS);
    Bfile_ReadFile(chart_file, buffer, BPM_SIZE, BPM_POS);
    chart_bpm = buffer[0] * 256 + buffer[1];
    Bfile_ReadFile(chart_file, buffer, NOTE_COUNT_SIZE, NOTE_COUNT_POS);
    chart_note_num = buffer[0] * 256 + buffer[1];
    Bfile_ReadFile(chart_file, buffer, SONG_LENGTH_SIZE, SONG_LENGTH_POS);
    song_length = buffer[0] * 256 + buffer[1];

    ML_clear_vram();

    locate(1, 1);
    Print((unsigned char *)"[SONG]");
    locate(1, 2);
    Print(chart_title);
    locate(1, 3);
    Print(&chart_title[21]);
    locate(1, 4);
    Print((unsigned char *)"[ARTIST]");
    locate(1, 5);
    Print(chart_artist);
    locate(1, 6);
    Print(&chart_artist[21]);
    locate(1, 7);
    Print((unsigned char *)"BPM:");
    locate(1, 8);
    Print((unsigned char *)"NOTE:");
    locate(12, 7);
    Print((unsigned char *)"TIME:");

    sprintf((char *)buffer, "%u", chart_bpm);
    locate(7, 7);
    Print(buffer);
    sprintf((char *)buffer, "%u", chart_note_num);
    locate(7, 8);
    Print(buffer);
    sprintf((char *)buffer, "%u:%02u", song_length / 60 / 40, (song_length / 40) % 60);
    locate(18, 7);
    Print(buffer);

    ML_display_vram();

    // Read notes
    for (i = 0; i < chart_note_num; i++)
    {
        Bfile_ReadFile(chart_file, buffer, NOTE_SIZE, FIRST_NOTE_POS + i * NOTE_SIZE);
        notes[i].start_time = buffer[0] * 256 + buffer[1];
        notes[i].end_time = buffer[2] * 256 + buffer[3];
        notes[i].type = buffer[4];
        notes[i].column = buffer[5];
        notes[i].score = SCORE_NOT_RATED;
    }
    ///////////////////////////
    // Construct linked list //
    ///////////////////////////

    column_start_ptr[0] = -1;
    column_start_ptr[1] = -1;
    column_start_ptr[2] = -1;
    column_start_ptr[3] = -1;
    column_now_ptr[0] = -1;
    column_now_ptr[1] = -1;
    column_now_ptr[2] = -1;
    column_now_ptr[3] = -1;

    for (column = 0; column < 4; column++)
    {
        for (i = 0; i < chart_note_num; i++)
        {
            if (notes[i].column == column)
            {
                if (column_start_ptr[column] == -1)
                {
                    column_start_ptr[column] = i;
                    column_now_ptr[column] = i;
                }
                else
                {
                    notes[column_now_ptr[column]].next = i;
                    notes[i].next = -1;
                    column_now_ptr[column] = i;
                }
            }
        }
    }

    GetKey(&key);
    if (key == KEY_CTRL_EXIT)
        return;

    select_speed();

////////////////
// Game start //
////////////////
GAME_START:

    time = 0;
    visible_note_start_ptr = 0;
    visible_note_end_ptr = -1;
    for (i = 0; i < 4; i++)
        column_now_ptr[i] = column_start_ptr[i];
    for (i = 0; i < chart_note_num; i++)
        notes[i].score = SCORE_NOT_RATED;

    miss_count = 0;
    good_count = 0;
    perfect_count = 0;
    combo = 0;
    max_combo = 0;

    while (1)
    {
        SetTimer(1, 25, &frame_timer);
        wait_frame = 1;
        ML_clear_vram();

        // Update key press
        key_state[0] = IsKeyDown(KEY_CHAR_7);
        key_state[1] = IsKeyDown(KEY_CHAR_8);
        key_state[2] = IsKeyDown(KEY_CTRL_DEL);
        key_state[3] = IsKeyDown(KEY_CTRL_AC);

        // Adding notes that appears
        while (visible_note_end_ptr < chart_note_num - 1 &&
               notes[visible_note_end_ptr + 1].start_time - VISIBLE <= time)
            visible_note_end_ptr++;

        // Removing notes that disappears
        while (notes[visible_note_start_ptr].start_time + DISAPPEAR < time)
            visible_note_start_ptr++;

        // Draw notes
        for (i = visible_note_start_ptr; i <= visible_note_end_ptr; i++)
        {
            if (notes[i].type == 'C') // Click
                ML_rectangle(
                    notes[i].column * COLUMN_WIDTH + 1,
                    63 - (notes[i].start_time - time) * 63 / VISIBLE - COLUMN_HEIGHT,
                    (notes[i].column + 1) * (COLUMN_WIDTH)-1,
                    63 - (notes[i].start_time - time) * 63 / VISIBLE,
                    1, ML_BLACK, ML_BLACK);

            if (notes[i].type == 'H') // Hold
                ML_rectangle(
                    notes[i].column * COLUMN_WIDTH + 1,
                    63 - (notes[i].end_time - time) * 63 / VISIBLE,
                    (notes[i].column + 1) * (COLUMN_WIDTH)-1,
                    63 - (notes[i].start_time - time) * 63 / VISIBLE,
                    1, ML_BLACK, ML_BLACK);
        }

        // Display note score
        sprintf((char *)buffer, "P %d", perfect_count);
        locate(14, 2);
        Print(buffer);
        sprintf((char *)buffer, "G %d", good_count);
        locate(14, 3);
        Print(buffer);
        sprintf((char *)buffer, "M %d", miss_count);
        locate(14, 4);
        Print(buffer);
        if (combo >= 5)
        {
            locate(14, 6);
            Print((unsigned char *)"COMBO");
            locate(14, 7);
            sprintf((char *)buffer, "[%d]", combo);
            Print(buffer);
        }

        draw_frame();
        draw_key_press_effect(key_state);
        note_state_machine(notes);
        draw_progress_bar(time, song_length);

        if (combo > max_combo)
            max_combo = combo;

        ML_display_vram();
        while (wait_frame) // Wait until next frame
        {
        }
        KillTimer(1);
        time++;

        if (time > song_length + 20 || IsKeyDown(KEY_CHAR_STORE)) // Game ends 0.5s after song ends
            break;

        // Pause
        if (IsKeyDown(KEY_CTRL_EXIT))
            switch (pause())
            {
            case KEY_CHAR_9:
                break;
            case KEY_CTRL_F1:
                goto GAME_START;
                break;
            case KEY_CTRL_F6:
                return;
                break;
            default:
                break;
            }
    }

    display_result(chart_title);
}
