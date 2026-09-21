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
        printf("ERROR: No list file path parameter given.\n");
        return 1;
    } else if (argv[2] == NULL) {
        printf("ERROR: No input file path parameter given.\n");
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

    char line[100];
    int numRecords;
    sscanf(fgets(line, 100, inputFile), "%d", &numRecords);

    for (int i = 0; i < numRecords; i++) {
        // TODO: Implement output
    }

    fclose(listFile);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
