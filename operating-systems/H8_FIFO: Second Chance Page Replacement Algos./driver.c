/**
 * ===============================================================================
 * PAGE REPLACEMENT ALGORITHMS
 * DRIVER PROGRAM (Reads input file and runs selected algorithm)
 * ===============================================================================
 */

#include "page_replacement.h"

/* ========================================================================================*/
/* UTILITY: Print usage information */
/* ========================================================================================*/

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s <input_file> <algorithm>\n", prog_name);
    fprintf(stderr, "\nAlgorithms:\n");
    fprintf(stderr, "  fifo           - First-In, First-Out\n");
    fprintf(stderr, "  second_chance  - Second Chance (Clock Algorithm)\n");
    // fprintf(stderr, "  lru            - Least Recently Used\n");
    // fprintf(stderr, "  optimal        - Optimal Page Replacement\n");
    fprintf(stderr, "\nExample:\n");
    fprintf(stderr, "  %s Testing/Testcases/simple.txt fifo\n", prog_name);
    fprintf(stderr, "  %s Testing/Testcases/simple.txt second_chance\n", prog_name);
}

/* ========================================================================================*/
/* Read input file and parse reference string and frame count */
/* ========================================================================================*/

int read_input(const char *filename, int *pages, int *frames) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
        return -1;
    }

    char buffer[MAX_LINE_LENGTH * 2];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    fclose(file);
    buffer[len] = '\0';

    // Find and parse reference string
    char *ptr = strstr(buffer, "=");
    if (!ptr) {
        fprintf(stderr, "Error: Invalid format\n");
        return -1;
    }
    ptr++; // skip '='

    int n = 0;
    while (*ptr && n < MAX_PAGES) {
        while (*ptr && !isdigit(*ptr) && *ptr != '\n') ptr++; // skip non-digits
        if (*ptr == '\n' || *ptr == '\0') break;
        pages[n++] = (int)strtol(ptr, &ptr, 10);
    }

    // Find and parse number of frames
    ptr = strstr(ptr, "=");
    if (ptr) {
        ptr++;
        *frames = (int)strtol(ptr, NULL, 10);
    } else {
        *frames = 3; // default
    }

    if (n == 0 || *frames <= 0 || *frames > MAX_FRAMES) {
        fprintf(stderr, "Error: Invalid input data\n");
        return -1;
    }

    return n;
}

/* ========================================================================================*/
/* Display current state of page frames */
/* ========================================================================================*/

void display_frames(int frame[], int frames) {
    printf("Frames: [");
    for (int i = 0; i < frames; i++) {
        if (frame[i] == -1) {
            printf(" _ ");
        } else {
            printf(" %d ", frame[i]);
        }
    }
    printf("]\n");
}

/* ========================================================================================*/
/* MAIN FUNCTION */
/* ========================================================================================*/

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    const char *algorithm = argv[2];

    // Read input
    int pages[MAX_PAGES];
    int frames;
    int n = read_input(input_file, pages, &frames);

    if (n <= 0) {
        fprintf(stderr, "Error: Failed to read input file.\n");
        return EXIT_FAILURE;
    }

    // Run selected algorithm
    if (strcmp(algorithm, "fifo") == 0) {
        fifo(pages, n, frames);
    } else if (strcmp(algorithm, "second_chance") == 0) {
        second_chance(pages, n, frames);
    } else if (strcmp(algorithm, "lru") == 0) {
        lru(pages, n, frames);
    } else if (strcmp(algorithm, "optimal") == 0) {
        optimal(pages, n, frames);
    } else {
        fprintf(stderr, "Error: Unknown algorithm '%s'\n", algorithm);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
