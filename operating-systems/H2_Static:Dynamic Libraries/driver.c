/**
 * @file driver.c
 * @brief Driver program that reads numbers from input file and processes them
 * @author Student Name
 * @date [Current Date]
 */

#include <stdio.h>
#include <stdlib.h>
#include "mathutils.h"

#define MAX_NUMBERS 100
#define INPUT_FILE "Testing/Testcases/input1.txt"

void process_number(int n);

int main(void) {
    FILE *input_file;
    int numbers[MAX_NUMBERS];
    int count = 0;
    int n;
    
    // Open input file
    input_file = fopen(INPUT_FILE, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", INPUT_FILE);
        fprintf(stderr, "Please ensure the file exists in the current directory.\n");
        return 1;
    }
    
    // Read numbers from file
    while (fscanf(input_file, "%d", &n) == 1 && count < MAX_NUMBERS) {
        numbers[count] = n;      
        count++;
    }

    // Close the input file
    fclose(input_file);

    // Check if any numbers were read
    if (count == 0) {
        printf("No valid numbers found in the input file.\n");
        return 1;
    }  
    
    // Process each number through all mathematical functions
    for (int i = 0; i < count; i++) {
        printf("---------------------------------------------------------------------------------\n");
        printf("Processing Number: %d\n", numbers[i]);        
        process_number(numbers[i]);       
    }    
    
    
    return 0;
}

/**
 * @brief Process a single number through all mathematical functions
 * @param n The number to process
 */
void process_number(int n) {    
    printf("1. Sum of first %d natural numbers: ", n);
    int sum = calculate_sum_of_n_numbers(n);
    if (sum != -1) {
        printf("%d\n", sum);
    }
    else {
        printf("Error(sum): Number must be positive\n");
    }
    
    printf("2. Factorial of %d: ", n);
    int fact = calculate_factorial(n);
    if (fact != -1) {
        printf("%d\n", fact);
    }
    else {
        printf("Error(factorial): Number must be positive\n");
    }

    printf("3. Fibonacci series with %d terms: ", n);   
    int fib = generate_fibonacci_series(n);
    if (fib == -1) {
        printf("Error(fibonacci): Number must be positive");
    } 
    printf("\n");      
}

