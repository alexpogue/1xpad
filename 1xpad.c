#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define GEN_KEY_MAX_CHUNK_MB 256
#define BYTES_PER_MB 1048576 /* 1024 * 1024 */

int strToUlong(unsigned long *val, char *str);
unsigned long getFileSize(FILE *fp);
void printFileError(char *file, char *rights);

int main(int argc, char *argv[]) {
    char *execName = argv[0];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-g <key length>] [<key file>]\n", execName);
        return 1;
    }

    if (argv[1][0] == '-' && argv[1][1] == 'g') {
        FILE *outFile = stdout;
        unsigned long keySizeMb;
        char *keySizeMbStr = argv[2];
        int intsPerMb, numIntsInKey, maxIntsInChunk, intsInFirstChunk, *chunk;
        chunk = NULL;
        if (!strToUlong(&keySizeMb, keySizeMbStr)) {
            fprintf(stderr, "Error: Couldn't interpret %s as unsigned long\n", keySizeMbStr);
            return 1;
        }
        srand(time(NULL));
        intsPerMb = BYTES_PER_MB / sizeof(int);
        numIntsInKey = keySizeMb * intsPerMb;
        maxIntsInChunk = GEN_KEY_MAX_CHUNK_MB * intsPerMb;
        intsInFirstChunk = min(numIntsInKey, maxIntsInChunk);
        chunk = malloc(intsInFirstChunk * sizeof(int));
        if (!chunk) {
            fprintf(stderr, "Error: malloc failed\n");
            return 1;
        }

        while (numIntsInKey > 0) {
            int numIntsInChunk = min(numIntsInKey, maxIntsInChunk);
            int i;
            for (i = 0; i < numIntsInChunk; i++)
                chunk[i] = rand();
            fwrite(chunk, sizeof(int), numIntsInChunk, outFile);
            numIntsInKey -= numIntsInChunk;
        }
        free(chunk);
    }
    else {
        FILE *keyFile, *messageFile, *outFile;
        unsigned long messageLen;
        if (argc < 2) {
            fprintf(stderr, "Usage: %s <key file>\n", execName);
            return 1;
        }

        keyFile = fopen(argv[1], "rb");
        if (!keyFile) printFileError(argv[1], "read");
        messageFile = stdin;
        if (!messageFile) printFileError("stdin", "read");
        outFile = stdout;
        if (!outFile) printFileError("stdout", "write");

        if (!keyFile || !messageFile || !outFile) {
            if (keyFile) fclose(keyFile);
            return 1;
        }

        int messageChar;
        while ((messageChar = fgetc(messageFile)) != EOF) {
            int keyChar = fgetc(keyFile);
            if (keyChar == EOF) {
                fprintf(stderr, "Error: message is longer than key\n");
                break;
            }
            char resultChar = (char)(messageChar ^ keyChar);
            fwrite(&resultChar, 1, 1, outFile);
        }
        fclose(keyFile);
    }
    return 0;
}

void printFileError(char *fileName, char *permission) {
    fprintf(stderr, "Error: couldn't open %s with %s permission\n", fileName, permission);
}

int strToUlong(unsigned long *val, char *str) {
    char *tmp;
    for (tmp = str; *tmp; tmp++) {
        if (!isdigit(*tmp))
            return 0;
    }
    *val = strtoul(str, NULL, 10);
    return 1;
}
