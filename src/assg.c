#include <stdio.h>

int parseInputFile(FILE *inputFile, int month[], int day[], int year[], int hour[], int minute[], char apm[], char type[], int location[]) {
    char warning[] = "WARNING: Failed to parse line %d of input.\n";
    
    char line[100];
    int i = 0;

    for (int n = 1; fgets(line, 100, inputFile) != NULL; n++) {
        if (!sscanf(line, "%u-%u-%u,%u:%u %cM,%c", &month[i], &day[i], &year[i], &hour[i], &minute[i], &apm[i], &type[i])) {
            printf(warning, n);
            continue;
        }

        if (type[i] == 'M') {
            location[i] = -1;
        } else if (!sscanf(line, "%*u-%*u-%*u,%*u:%*u %*cM,%*c,%u", &location[i])) {
            printf(warning, n);
            continue;
        }

        i++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("ERROR: 2 input parameters expected; %d given.\n", argc);
        return 1;
    }

    FILE *listFile = fopen(argv[1], "r");
    if (listFile == NULL) {
        printf("ERROR: List file %s failed to open.\n", argv[1]);
        return 1;
    }

    FILE *inputFile = fopen(argv[2], "r");
    if (inputFile == NULL) {
        printf("ERROR: Input file %s failed to open.\n", argv[2]);
        return 1;
    }

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("ERROR: Failed to create output file output.txt.\n");
        return 1;
    }

    int numRecords;
    if (!fscanf(inputFile, "%d", &numRecords)) {
        printf("ERROR: Could not read number of records from file.\n");
        return 1;
    }

    int month[numRecords];
    int day[numRecords];
    int year[numRecords];
    int hour[numRecords];
    int minute[numRecords];
    char apm[numRecords];
    char type[numRecords];
    int location[numRecords];

    parseInputFile(inputFile, month, day, year, hour, minute, apm, type, location);

    fclose(listFile);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
