#define bool char
#define true 1
#define false 0
#define toggle(x) x ^= 1

#define String unsigned char*
#define StringCasio unsigned short* 

#define MAX_FILE_PATH_STRING_LENGTH sizeof(short) * 28
#define MAX_FILE_PATH_CHARACTER 28
#define FILE_PATH_DRIVE_PART_LENGTH 7

#define KEY_SWITCH_LETTER_CASE KEY_CTRL_F6

enum FileSource { Flash, SdCard };

// Prompt the user to enter a file path.
// Returns a pointer to the file path entered, or null if user cancels.
StringCasio PromptFilePath(String title);

// Show a multiple pages text.
// User can press the arrow key to browse between pages, or press EXIT to quit.
void ShowMultiplePagesText(
    char ***pages,
    unsigned char numberOfPages,
    unsigned char numberOfLinesInOnePage);
