/********************************************************************
* FILENAME : get-version.c
*
* DESCRIPTION :
*       Import Linux version from zimage File.
*
* AUTHOR : KyuHyuk Lee <lee@kyuhyuk.kr>   START DATE : 15 Dec 2018
*
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define KERNEL_FILENAME "zImage"

const unsigned char startPatterns[] = {0x4C,0x69,0x6E,0x75,0x78,0x20,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E};
const unsigned char endPatterns[] = {0x0A,0x00};

unsigned long int getStartOffset(unsigned char *buffer, unsigned int inputFileSize);
unsigned long int getEndOffset(unsigned long int startOffset, unsigned char *buffer, unsigned int inputFileSize);

int main() {
        FILE *fp = fopen(KERNEL_FILENAME, "r");;
        unsigned char *buffer;
        unsigned char *version;
        unsigned int count;
        unsigned int inputFileSize;
        unsigned int versionLen;
        unsigned long int startOffset, endOffset;

        fseek(fp, 0, SEEK_END);
        inputFileSize = ftell(fp);
        buffer = malloc(inputFileSize + 1);
        memset(buffer, 0, inputFileSize + 1);
        fseek(fp, 0, SEEK_SET);
        count = fread(buffer, inputFileSize, 1, fp);
        printf("Filename : %s, Size: %d\n", KERNEL_FILENAME, inputFileSize);
        fclose(fp);

        startOffset = getStartOffset(buffer, inputFileSize);
        endOffset = getEndOffset(startOffset, buffer, inputFileSize);
        versionLen = endOffset - startOffset;

        version = malloc(versionLen + 1);
        memset(version, 0, versionLen + 1);
        memcpy(version, buffer + startOffset, versionLen + 1 );
        printf("%s\n", version);
        free(version);
        return 0;
}

unsigned long int getStartOffset(unsigned char *buffer, unsigned int inputFileSize) {
        int i = 0;
        for(i=0; i<inputFileSize; i++) {
                if(buffer[i] == startPatterns[0] &&
                   buffer[i+1] == startPatterns[1] &&
                   buffer[i+2] == startPatterns[2] &&
                   buffer[i+3] == startPatterns[3] &&
                   buffer[i+4] == startPatterns[4] &&
                   buffer[i+5] == startPatterns[5] &&
                   buffer[i+6] == startPatterns[6] &&
                   buffer[i+7] == startPatterns[7] &&
                   buffer[i+8] == startPatterns[8] &&
                   buffer[i+9] == startPatterns[9] &&
                   buffer[i+10] == startPatterns[10] &&
                   buffer[i+11] == startPatterns[11] &&
                   buffer[i+12] == startPatterns[12] ) {
                        printf("Start Offset : 0x%X\n", i + 8);
                        return i;
                }
        }
        fprintf(stderr,"'IKCFG_ST' could not be found.");
        exit(EXIT_FAILURE);
}

unsigned long int getEndOffset(unsigned long int startOffset, unsigned char *buffer, unsigned int inputFileSize) {
        int i = 0;
        for(i=startOffset; i<inputFileSize; i++) {
                if(buffer[i] == endPatterns[0] &&
                   buffer[i+1] == endPatterns[1] ) {
                        printf("End Offset : 0x%X\n", i);
                        return i;
                }
        }
        fprintf(stderr,"'IKCFG_ED' could not be found.");
        exit(EXIT_FAILURE);
}
