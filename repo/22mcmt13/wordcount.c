#include <stdio.h>
#include <string.h>

// Finding Number of new line characters in a buff
int find_count_of_lines(const char *str)
{
    int lines = 0, a = 0;
    for(a = 0; str[a]!= '\0'; a++) 
    {
        if(str[a] == '\n') 
        {
            lines += 1;
        }
    }
    return lines;
}

int find_count_of_words(const char *str) {
    int words = 0, a = 0;
    int inWord = 0; // Use this as a flag to track if we're inside a word or not

    for(; str[a] != '\0'; a++) {
        if(str[a] == ' ' || str[a] == '\n' || str[a]== '\r' || str[a] == '\t') {
            if(inWord) { // We are at the end of a word
                words++;
                inWord = 0; // Reset the flag
            }
        } else {
            inWord = 1;
        }
    }
    // Check for the last word in the file without any whitespace after
    if(inWord) {
        words++;
    }

    return words;
}

int txt_my_wc(FILE *fp, int *lines_p, int *words_p, int *chars_p) {
    int lines = 0, words = 0, chars = 0;
    char buffer[1024] = {0x00};
    size_t bytes = 0;

    fseek(fp, 0L, SEEK_SET);
    do {
        memset(buffer, 0x00, sizeof(buffer));
        bytes = fread(buffer, sizeof(char), sizeof(buffer) - 1, fp);
        if(bytes > 0) {
            lines += find_count_of_lines(buffer);
            words += find_count_of_words(buffer);
            chars += bytes;
        }
    } while(bytes > 0);

    *lines_p = lines;
    *words_p = words;
    *chars_p = chars;
    return chars;
}

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    int lines = 0, words = 0, chars = 0;

    if(argc != 2) {
        fprintf(stderr, "USAGE: %s <wordcount> \n", argv[0]);
        return -1;
    }

    fp = fopen(argv[1], "r");
    if(!fp) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", argv[1]);
        return -1;
    }

    txt_my_wc(fp, &lines, &words, &chars);
    printf("  %d  %d  %d  %s\n", lines, words, chars, argv[1]);
    fclose(fp);
    return 0;
}
