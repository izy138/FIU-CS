/*
 * Mathematical Utilities Library - Student Implementation Template
 *
 * INSTRUCTIONS:
 * Complete the implementation of the following functions according to specifications.
 * Follow the function signatures exactly as provided.
 * Handle all edge cases and error conditions as specified.
 *
 * REQUIREMENTS:
 * - All functions must handle invalid inputs appropriately
 * - Use appropriate data types to prevent overflow
 * - Follow good coding practices (comments, variable names, etc.)
 * - Test your functions thoroughly before submission
 */

#include "mathutils.h"

/**
 * Calculates the sum of first n natural numbers
 *
 * SPECIFICATION:
 * - Calculate sum of numbers from 1 to n (inclusive)
 * - Use the mathematical formula: sum = n * (n + 1) / 2
 * - Return -1 for invalid input (n <= 0)
 *
 * EXAMPLES:
 * calculate_sum_of_n_numbers(5) should return 15 (1+2+3+4+5)
 * calculate_sum_of_n_numbers(0) should return -1
 * calculate_sum_of_n_numbers(-3) should return -1
 *
 * @param n: Number of natural numbers to sum
 * @return: Sum of first n natural numbers, or -1 if invalid input
 */
long long calculate_sum_of_n_numbers(int n)
{
    // TODO: Implement this function
    //
    // check for negative or zero
    if (n <= 0)
        return -1;

    // cast int into long long
    return ((long long)n * (n + 1) / 2);
}

/**
 * Calculates the factorial of a given number (n!)
 *
 * SPECIFICATION:
 * - Calculate n! = n × (n-1) × (n-2) × ... × 1
 * - 0! = 1 and 1! = 1 by definition
 * - Return -1 for invalid input (n < 0)
 * - Use iterative approach (not recursive)
 *
 * EXAMPLES:
 * calculate_factorial(5) should return 120 (5×4×3×2×1)
 * calculate_factorial(0) should return 1
 * calculate_factorial(1) should return 1
 * calculate_factorial(-2) should return -1
 *
 * @param n: Number to calculate factorial for
 * @return: Factorial of n, or -1 if invalid input
 */
long long calculate_factorial(int n)
{
    // TODO: Implement this function
    //
    // check for negative or zero / one
    if (n == 0 || n == 1)
        return 1;
    if (n < 0)
        return -1;

    // you start i at 2 because 1 x 2 is the first operation
    long long result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i; // multiply result by i
    }

    return result;
}

/**
 * Generates and prints the first n numbers in the Fibonacci series
 *
 * SPECIFICATION:
 * - Print Fibonacci sequence: 0, 1, 1, 2, 3, 5, 8, 13, ...
 * - Format: comma-separated values (no trailing comma or newline)
 * - Return -1 for invalid input (n <= 0)
 * - Return 0 for successful execution
 *
 * OUTPUT EXAMPLES:
 * generate_fibonacci_series(1) should print: "0"
 * generate_fibonacci_series(2) should print: "0, 1"
 * generate_fibonacci_series(6) should print: "0, 1, 1, 2, 3, 5"
 * generate_fibonacci_series(0) should return -1 (print nothing)
 *
 * @param n: Count of Fibonacci numbers to generate
 * @return: 0 on success, -1 if invalid input
 */

long long fibonacci_recursive(int n)
{
    // base case for recursion
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    // recursive case: f(n) = f(n-1) + f(n-2)
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}
int generate_fibonacci_series(int n)
{
    // TODO: Implement this function
    //
    // check for negative or zero
    if (n <= 0)
    {
        return -1;
    }

    // print fibonacci and call the recursive function to print the number
    for (int i = 0; i < n; i++)
    {
        // calls the recursive function and sets it to num so we can print it in order till n
        long long num = fibonacci_recursive(i);

        if (i == 0)
        {
            printf("%lld", num); // first number printed
        }
        else
        {
            printf(", %lld", num); // every consequetive num includes a comma
        }
    }

    return 0;
}
