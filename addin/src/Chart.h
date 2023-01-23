#include "FxNeue.h"

struct Chart
{
    String SongName;
    String SongArtist;
    int BPM;
    // A chart contains 4 columns,
    // each column contains up to 2000 notes.
    struct Note NoteColumns[4][2000];
    // Actual number of notes
    int NoteCount;
    int SongDuration;
};

struct Note
{
    enum NoteType Type : 4;
    unsigned char ScoreState : 4;
    short Next;
    unsigned int TimeStart;
    unsigned int TimeEnd;
};

enum NoteType { Click, Hold };

struct Chart* NewChartFromPath(StringCasio* path);
