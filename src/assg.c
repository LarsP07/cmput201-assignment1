#include <stdio.h>

int parseInputFile(FILE *inputFile, int month[], int day[], int year[], int hour[], int minute[], char type[], int location[]) {
    char line[100];
    int i = 0;

    char apm;

    for (int n = 0; fgets(line, 100, inputFile); n++) {
        if (!sscanf(line, "%u-%u-%u,%u:%u %cM,%c", &month[i], &day[i], &year[i], &hour[i], &minute[i], &apm, &type[i])) {
            return -1;
        }

        if (type[i] == 'M') {
            location[i] = -1;
        } else if (!sscanf(line, "%*u-%*u-%*u,%*u:%*u %*cM,%*c,%u", &location[i])) {
            return -1;
        }

        if (apm == 'P') hour[i] += 12;

        i++;
    }

    return i;
}

int findRoute(FILE *listFile, int entry, int exit, char schedule[], char entryStop[], char exitStop[]) {
    FILE *scheduleFile;
    char path[100];
    
    rewind(listFile);

    int location;
    int foundRoute = 0;

    char tempSchedule[100];
    char tempEntryStop[100];
    char tempExitStop[100];

    char line[100];

    while (fgets(path, 100, listFile)) {
        // Quick way to eliminate trailing newline character.
        sscanf(path, "%s", path);
        scheduleFile = fopen(path, "r");

        fgets(tempSchedule, 100, scheduleFile);

        int foundEntry = 0;

        for (int i = 1; fgets(line, 100, scheduleFile) && (!foundEntry || !foundRoute); i++) {
            sscanf(line, "%u", &location);
            if (!foundEntry && location == entry) {
                sscanf(line, "%s", tempEntryStop);
                foundEntry = 1;
            } else if (location == exit) {
                sscanf(line, "%s", tempExitStop);
                foundRoute = 1;
            }
        }

        fclose(scheduleFile);

        if (foundRoute) break;
    }

    if (foundRoute) {
        sscanf(tempSchedule, "%s", schedule);
        sscanf(tempEntryStop, "%s", entryStop);
        sscanf(tempExitStop, "%s", exitStop);
    }

    return foundRoute;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("2 input parameters expected, %d given\n", argc);
        return 1;
    }

    FILE *listFile = fopen(argv[1], "r");
    if (listFile == NULL) {
        printf("List file %s failed to open\n", argv[1]);
        return 1;
    }

    FILE *inputFile = fopen(argv[2], "r");
    if (inputFile == NULL) {
        printf("Input file %s failed to open\n", argv[2]);
        return 1;
    }

    int numRecords;
    if (!fscanf(inputFile, "%d", &numRecords)) {
        printf("Invalid data format");
        return 1;
    }

    int month[numRecords];
    int day[numRecords];
    int year[numRecords];
    int hour[numRecords];
    int minute[numRecords];
    char type[numRecords];
    int location[numRecords];

    int parsedRecords = parseInputFile(inputFile, month, day, year, hour, minute, type, location);
    if (parsedRecords == -1) {
        printf("Invalid data format\n");
        fclose(inputFile);
        return 1;
    }

    char schedule[100];
    char entryStop[100];
    char exitStop[100];

    for (int i = 0; i < numRecords; i += 2) {
        if (findRoute(listFile, location[i + 1], location[i], schedule, entryStop, exitStop)) printf("%s %s %s\n", schedule, entryStop, exitStop);
    }

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Failed to create output file output.txt\n");
        return 1;
    }

    fclose(listFile);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
