/**
 * @file driver.c
 * @brief Driver program with comprehensive diagnostics to debug file reading issue
 * @author Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_controller.h"

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 64

/**
 * @brief ISO C17-compatible strdup replacement
 */
static char *xstrdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (!copy)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, s, len);
    return copy;
}

/**
 * @brief Parses a command line string into a heap-allocated argv array
 * @param line input string
 * @return char** NULL-terminated argv array, must be freed with free_args()
 */
char **parse_command(const char *line)
{
    char **args = calloc(MAX_ARGS, sizeof(char *));
    if (!args)
    {
        perror("calloc failed");
        exit(EXIT_FAILURE);
    }

    char *line_copy = xstrdup(line);
    int argc = 0;
    char *token = strtok(line_copy, " \t\n\r");

    while (token && argc < MAX_ARGS - 1)
    {
        args[argc] = xstrdup(token);
        argc++;
        token = strtok(NULL, " \t\n\r");
    }
    args[argc] = NULL;

    free(line_copy);
    return args;
}

/**
 * @brief Frees a dynamically allocated argv array
 */
void free_args(char **args)
{
    if (!args) return;

    for (int i = 0; args[i] != NULL; i++)
    {
        free(args[i]);
    }
    free(args);
}

int main(int argc, char *argv[])
{
    FILE *input_file;
    char **lines;
    int line_count = 0;
    int line_capacity = 10;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open file
    input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // Read file into memory
    lines = malloc(line_capacity * sizeof(char *));
    if (!lines)
    {
        perror("malloc failed");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), input_file))
    {
        if (line_count >= line_capacity)
        {
            line_capacity *= 2;
            char **tmp = realloc(lines, line_capacity * sizeof(char *));
            if (!tmp)
            {
                perror("realloc failed");
                fclose(input_file);
                return EXIT_FAILURE;
            }
            lines = tmp;
        }
        lines[line_count] = xstrdup(buffer);
        line_count++;
    }

    fclose(input_file);

    printf("Starting Process Controller\n");
    fflush(stdout);

    // Process lines from memory
    for (int i = 0; i < line_count; i++)
    {
        char *trimmed = lines[i];
        while (*trimmed == ' ' || *trimmed == '\t')
            trimmed++;

        if (*trimmed == '\0' || *trimmed == '\n' || *trimmed == '#')
            continue;

        char **args = parse_command(trimmed);
        if (args[0] != NULL)
        {
            execute_command(args);
            free_args(args);
        }
    }

    // Free stored lines
    for (int i = 0; i < line_count; i++)
    {
        free(lines[i]);
    }
    free(lines);

    printf("All processes completed\n");
    return EXIT_SUCCESS;
}
