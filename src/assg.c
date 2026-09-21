#include <stdio.h>

void removeTrailingNewline(char *str) {
    if (str[0] == '\0') return;
    
    int i;
    for (i = 0; str[i] != '\0'; i++);
    if (str[i - 1] == '\n') str[i - 1] = '\0';

    return;
}

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        printf("ERROR: No path files parameter given.\n");
        return 1;
    }

    FILE *schedulesFile = fopen(argv[1], "r");

    if (schedulesFile == NULL) {
        printf("ERROR: %s failed to open.\n", argv[1]);
        return 1;
    }

    int i;
    char schedulePaths[1000][50];
    for (i = 0; fgets(schedulePaths[i], 50, schedulesFile) != NULL; i++) {
        sscanf(schedulePaths[i], "%s", schedulePaths[i]);
    }

    int count = i;

    fclose(schedulesFile);

    printf("Processing %d schedule files...\n", count);

    FILE *schedule;

    for (i = 0; i < count; i++) {
        schedule = fopen(schedulePaths[i], "r");
        if (schedule == NULL) {
            printf("ERROR: Could not open schedule with path %s.\n", schedulePaths[i]);
            continue;
        }

        char direction;
        int j;
        for (j = 0; schedulePaths[i][j] != '\0'; j++);
        direction = schedulePaths[i][j - 6];

        char line[50];

        int routeNo;
        int numStops;

        sscanf(fgets(line, 50, schedule), "%d%*s%d", &routeNo, &numStops);

        fclose(schedule);

        printf("schedule #%d is route %d it has %d stops and is in the %c direction\n", i+1, routeNo, numStops, direction);
    }

    return 0;
}
