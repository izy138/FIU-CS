#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <stdio.h>

/**
 * @brief Calculate the sum of first n natural numbers
 * @param n The number of natural numbers to sum
 * @return The sum of first n natural numbers, or -1 if n <= 0
 */
long long calculate_sum_of_n_numbers(int n);

/**
 * @brief Calculate the factorial of n
 * @param n The number to calculate factorial for
 * @return The factorial of n, or -1 if n < 0
 */
long long calculate_factorial(int n);

/**
 * @brief Generate and print Fibonacci series with n terms
 * @param n The number of terms in Fibonacci series
 * @return 0 on success, -1 if n <= 0
 */
int generate_fibonacci_series(int n);

#endif