#include <stdio.h>

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

        char routeNo[5];
        sscanf(schedulePaths[i], "simplified_schedules/route_%4s", routeNo);
        for (int j = 0; j < 4; j++) {
            if (routeNo[j] == '_') {
                routeNo[j] = '\0';
                break;
            }
        }

        int numStops;
        char line[50];
        fgets(line, 50, schedule);

        sscanf(line, "%*d%*s%d", &numStops);

        fclose(schedule);

        printf("schedule #%d is route %s it has %d stops and is in the %c direction\n", i+1, routeNo, numStops, direction);
    }

    return 0;
}
