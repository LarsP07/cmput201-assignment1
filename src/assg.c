#include <stdio.h>

char *ISOFormat(char *buffer, int month, int day, int year, int hour, int minute) {
    sprintf(buffer, "%4d-%02d-%02dT%02d:%02d:00", year, month, day, hour, minute);
    return buffer;
}

char *getTravelTime(char *buffer, int ihour, int iminute, int fhour, int fminute) {
    int hour = fhour - ihour;
    int minute = fminute - iminute;
    if (minute < 0) {
        minute += 60;
        hour--;
    }
    
    if (hour < 0) {
        hour += 24;
    }

    sprintf(buffer, "%d:%02d:00", hour, minute);
}

int parseInputFile(FILE *inputFile, int month[], int day[], int year[], int hour[], int minute[], char type[], int location[]) {
    char line[100];

    char apm;

    int i;

    // Done to skip the first line. Much easier than dealing with fpos.
    fgets(line, 100, inputFile);

    for (i = 0; fgets(line, 100, inputFile); i++) {
        if (!sscanf(line, "%u-%u-%u,%u:%u %cM,%c", &month[i], &day[i], &year[i], &hour[i], &minute[i], &apm, &type[i])) {
            return -1;
        }

        if (type[i] == 'M') {
            location[i] = -1;
        } else if (!sscanf(line, "%*u-%*u-%*u,%*u:%*u %*cM,%*c,%u", &location[i])) {
            return -1;
        }

        if (apm == 'P' && hour[i] != 12) hour[i] += 12;
        else if (apm == 'A' && hour[i] == 12) hour[i] = 0;
    }

    return i;
}

int findRoute(FILE *listFile, int entry, int exit, char route[], char entryStop[], char exitStop[]) {
    FILE *scheduleFile;
    char path[100];
    
    rewind(listFile);

    int location;
    int foundRoute = 0;

    char tempSchedule[100];
    char tempEntryStop[100];
    char tempExitStop[100];

    char line[100];

    while (fgets(path, 100, listFile) && !foundRoute) {
        // Quick way to eliminate trailing newline character.
        // NOTE: This only works if your string has NO WHITESPACE CHARACTERS other than the newline.
        // Learned that the hard way.
        sscanf(path, "%s", path);
        scheduleFile = fopen(path, "r");

        fgets(tempSchedule, 100, scheduleFile);

        int foundEntry = 0;

        while (fgets(line, 100, scheduleFile) && (!foundEntry || !foundRoute)) {
            sscanf(line, "%u", &location);
            if (!foundEntry && location == entry) {
                sscanf(line, "%s", tempEntryStop);
                foundEntry = 1;
            } else if (foundEntry && location == exit) {
                sscanf(line, "%s", tempExitStop);
                foundRoute = 1;
            }
        }

        fclose(scheduleFile);
    }

    if (foundRoute) {
        int routeNo;
        sscanf(tempSchedule, "%d", &routeNo);
        char direction;
        for (int i = 0; i < 100; i++) {
            if (path[i] == 'B') {
                direction = path[i - 1];
                break;
            }
        }

        sprintf(route, "%d %c", routeNo, direction);
        sprintf(entryStop, "%s", tempEntryStop);
        sprintf(exitStop, "%s", tempExitStop);
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

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Failed to create output file output.txt\n");
        return 1;
    }

    char schedule[100];
    char entryStop[100];
    char exitStop[100];
    char entryTime[100];
    char exitTime[100];
    char duration[100];

    for (int i = numRecords - 1; i >= 0; i -= 2) {
        if (!findRoute(listFile, location[i], location[i - 1], schedule, entryStop, exitStop)) {
            printf("Invalid stop ID");
            return 1;
        }

        ISOFormat(entryTime, month[i], day[i], year[i], hour[i], minute[i]);
        ISOFormat(exitTime, month[i - 1], day[i - 1], year[i - 1], hour[i - 1], minute[i - 1]);
        getTravelTime(duration, hour[i], minute[i], hour[i - 1], minute[i - 1]);

        fprintf(outputFile, "%s,%s,%s,%s,%s,%s\n", schedule, entryStop, exitStop, entryTime, exitTime, duration);
    }

    fclose(listFile);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
