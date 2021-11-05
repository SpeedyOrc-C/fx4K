#define TITLE_POS 0x0
#define TITLE_SIZE 32
#define ARTIST_POS 0x20
#define ARTIST_SIZE 32
#define BPM_POS 0x40
#define BPM_SIZE 2
#define NOTE_COUNT_POS 0x42
#define NOTE_COUNT_SIZE 2
#define SONG_LENGTH_POS 0x44
#define SONG_LENGTH_SIZE 2
#define FIRST_NOTE_POS 0x50
#define NOTE_SIZE 8

#define GOOD 4
#define PERFECT 4
#define APPEAR 10
#define DISAPPEAR 200
#define COLUMN_WIDTH 16
#define COLUMN_HEIGHT 8

#define SCORE_NOT_RATED 0
#define SCORE_RATE_READY 1
#define SCORE_MISS 2
#define SCORE_GOOD 3
#define SCORE_PERFECT 4

struct Note
{
    unsigned char type;
    unsigned char column;
    short next;
    short start_time;
    short end_time;
    unsigned char score;
};

void play(unsigned char *file_path);
