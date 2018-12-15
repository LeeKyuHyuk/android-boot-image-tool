#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define KERNEL_FILENAME "zImage"
#define KERNEL_CONFIG_FILENAME "config.gz"

const unsigned char startPatterns[] = {0x49,0x4b,0x43,0x46,0x47,0x5f,0x53,0x54};
const unsigned char endPatterns[] = {0x49,0x4b,0x43,0x46,0x47,0x5f,0x45,0x44};

unsigned long int getStartOffset(unsigned char *buffer, unsigned int inputFileSize);
unsigned long int getEndOffset(unsigned long int startOffset, unsigned char *buffer, unsigned int inputFileSize);

int main() {
        FILE *fp;
        unsigned char *buffer;
        unsigned char *configFile;
        unsigned int count;
        unsigned int inputFileSize;
        unsigned int configFileSize;
        unsigned long int startOffset, endOffset;

        fp  = fopen(KERNEL_FILENAME, "r");
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
        configFileSize = endOffset - startOffset;
        printf("config.gz file size : %d\n", configFileSize);

        fp = fopen (KERNEL_CONFIG_FILENAME, "w");
        configFile = malloc(configFileSize + 1);
        memset(configFile, 0, configFileSize + 1);
        memcpy(configFile, buffer + startOffset, configFileSize + 1 );
        fwrite(configFile, configFileSize, 1, fp);
        fclose(fp);
        free(configFile);
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
                   buffer[i+7] == startPatterns[7] ) {
                        printf("Start Offset : 0x%X\n", i + 8);
                        return i + 8;
                }
        }
        fprintf(stderr,"'IKCFG_ST' could not be found.");
        exit(EXIT_FAILURE);
}

unsigned long int getEndOffset(unsigned long int startOffset, unsigned char *buffer, unsigned int inputFileSize) {
        int i = 0;
        for(i=startOffset; i<inputFileSize; i++) {
                if(buffer[i] == endPatterns[0] &&
                   buffer[i+1] == endPatterns[1] &&
                   buffer[i+2] == endPatterns[2] &&
                   buffer[i+3] == endPatterns[3] &&
                   buffer[i+4] == endPatterns[4] &&
                   buffer[i+5] == endPatterns[5] &&
                   buffer[i+6] == endPatterns[6] &&
                   buffer[i+7] == endPatterns[7] ) {
                        printf("End Offset : 0x%X\n", i);
                        return i;
                }
        }
        fprintf(stderr,"'IKCFG_ED' could not be found.");
        exit(EXIT_FAILURE);
}
