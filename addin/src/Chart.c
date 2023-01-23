#include "Chart.h"
#include "fxlib.h"
#include "stdlib.h"

struct Chart* NewChartFromPath(StringCasio* path)
{
    int i;
    struct Chart chart;

    int file = Bfile_OpenFile(path, _OPENMODE_READ);
    if (file < 0) return NULL;

    chart.SongName = malloc(32);
    chart.SongArtist = malloc(32);
    chart.NoteColumns;

    
}
