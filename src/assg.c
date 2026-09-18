#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *schedulesFile = fopen(argv[1], "r");

    if (schedulesFile == NULL) {
        printf("ERROR: list_file.txt failed to open.\n");
        return 1;
    }

    char schedulePaths[1000][50];
    for (int i = 0; fgets(schedulePaths[i], 50, schedulesFile) != NULL; i++) {
        printf("%s", schedulePaths[i]);
    }

    fclose(schedulesFile);

    return 0;
}
