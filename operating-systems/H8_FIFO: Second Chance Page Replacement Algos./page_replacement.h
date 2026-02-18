/*
 * ===============================================================================
 * PAGE REPLACEMENT ALGORITHMS - HEADER FILE
 * ⚠️ Do not modify this file contents!
 * ===============================================================================
 * 
 * This header defines all data structures, constants, and function prototypes
 * for the page replacement algorithms assignment.
 * 
 * @author Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 * ===============================================================================
 */

#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* ========================================================================================*/
/* CONSTANTS */
/* ========================================================================================*/

#define MAX_FRAMES 20           // Maximum number of page frames
#define MAX_PAGES 1000          // Maximum number of pages in reference string
#define MAX_LINE_LENGTH 1024    // Maximum line length for file reading

/* ========================================================================================*/
/* FUNCTION PROTOTYPES - PROVIDED BY DRIVER */
/* ========================================================================================*/

/**
 * @brief Read input file and parse reference string and frame count
 * 
 * Input file format:
 *   Reference string = 1,0,7,1,0,2,1,2,3,0,3,2,4,0,3,0,2,1
 *   Number of frame = 3
 * 
 * @param filename Path to input file
 * @param pages Array to store page reference string
 * @param frames Pointer to store number of frames
 * @return Number of pages read, or -1 on error
 */
int read_input(const char *filename, int *pages, int *frames);

/**
 * @brief Display current state of page frames
 * 
 * Format: [ 1  0  7 ]  or  [ 1  _  _ ]  for empty frames
 * 
 * @param frame Array of page frames
 * @param frames Number of frames
 */
void display_frames(int frame[], int frames);

/* ========================================================================================*/
/* FUNCTION PROTOTYPES - STUDENTS MUST IMPLEMENT */
/* ========================================================================================*/

/**
 * @brief First-In, First-Out (FIFO) page replacement algorithm
 * 
 * This function implements the FIFO algorithm:
 * 1. Initialize all frames to -1 (empty)
 * 2. Initialize victim pointer to 0
 * 3. For each page reference:
 *    a. Check if page is in memory (page hit)
 *    b. If not in memory (page fault):
 *       - Replace page at victim pointer position
 *       - Advance victim pointer circularly: k = (k + 1) % frames
 * 4. Display step-by-step execution
 * 5. Display summary statistics
 * 
 * Output format for each step:
 *   Page X loaded. Page fault occurred. Frames: [ 1  0  _ ]
 *   Page Y already in memory. No page fault. Frames: [ 1  0  7 ]
 * 
 * Summary format:
 *   FIFO Summary
 *   ==============
 *   Reference String: 1 0 7 1 0 2
 *   Page Faults:      Y Y Y N N Y
 *   Total Page Faults: 4
 *   Hit Ratio (Numbe_of_Page_Hits / Total_Number_of_Pages): 33.33%
 * 
 * @param pages Array containing page reference string
 * @param n Number of page references
 * @param frames Number of available page frames
 */
void fifo(int pages[], int n, int frames);

/**
 * @brief Second Chance (Clock) page replacement algorithm
 * 
 * This function implements the Second Chance algorithm:
 * 1. Initialize all frames to -1 (empty)
 * 2. Initialize all reference bits to 0
 * 3. Initialize victim pointer to 0
 * 4. For each page reference:
 *    a. Check if page is in memory
 *    b. If page hit: Set reference bit to 1 (DO NOT move pointer)
 *    c. If page fault (empty frame available):
 *       - Load page into empty frame
 *       - Set reference bit to 1
 *    d. If page fault (all frames full):
 *       - Search for victim starting from current pointer:
 *         * If reference bit = 0: Replace page, advance pointer
 *         * If reference bit = 1: Clear to 0, advance pointer, continue
 *       - Load new page and set reference bit to 1
 * 5. Display step-by-step execution
 * 6. Display summary statistics
 * 
 * Output format for each step:
 *   Page X loaded into empty frame. Frames: [ 1  _  _ ]
 *   Page Y already in memory. Reference bit set. Frames: [ 1  0  7 ]
 *   Page Z replaces page with reference bit 0. Frames: [ 2  0  7 ]
 * 
 * Summary format:
 *   Second Chance Summary
 *   ======================
 *   Reference String: 1 0 7 1 0 2
 *   Page Faults:      Y Y Y N N Y
 *   Total Page Faults: 4
 *   Hit Ratio: 33.33%
 * 
 * @param pages Array containing page reference string
 * @param n Number of page references
 * @param frames Number of available page frames
 */
void second_chance(int pages[], int n, int frames);

/**
 * @brief Least Recently Used (LRU) page replacement algorithm
 * 
 * This function implements the LRU algorithm:
 * 1. Initialize all frames to -1 (empty)
 * 2. Initialize all last_used times to 0
 * 3. For each page reference at time step i:
 *    a. Check if page is in memory
 *    b. If page hit: Update last_used[j] = i for frame j
 *    c. If page fault (empty frame available):
 *       - Load page into empty frame
 *       - Set last_used[j] = i
 *    d. If page fault (all frames full):
 *       - Find frame with minimum last_used value (LRU)
 *       - Replace that frame with new page
 *       - Update last_used[j] = i
 * 4. Display step-by-step execution
 * 5. Display summary statistics
 * 
 * Output format for each step:
 *   Page X loaded into empty frame. Frames: [ 1  _  _ ]
 *   Page Y already in memory. Updated access time. Frames: [ 1  0  7 ]
 *   Page Z replaces page A (least recently used). Frames: [ 2  0  7 ]
 * 
 * Summary format:
 *   LRU Summary
 *   ===============
 *   Reference String: 1 0 7 1 0 2
 *   Page Faults:      Y Y Y N N Y
 *   Total Page Faults: 4
 *   Hit Ratio: 33.33%
 * 
 * @param pages Array containing page reference string
 * @param n Number of page references
 * @param frames Number of available page frames
 */
void lru(int pages[], int n, int frames);

/**
 * @brief Optimal page replacement algorithm
 * 
 * This function implements the Optimal algorithm:
 * 1. Initialize all frames to -1 (empty)
 * 2. For each page reference at position i:
 *    a. Check if page is in memory
 *    b. If page hit: No action needed
 *    c. If page fault (empty frame available):
 *       - Load page into empty frame
 *    d. If page fault (all frames full):
 *       - For each page in frames:
 *         * Scan forward from position i+1 to find next use
 *         * If not found, set next_use = n (never used again)
 *       - Select page with maximum next_use (farthest in future)
 *       - Replace that frame with new page
 * 3. Display step-by-step execution
 * 4. Display summary statistics
 * 
 * Output format for each step:
 *   Page X loaded into empty frame. Frames: [ 1  _  _ ]
 *   Page Y already in memory. No page fault. Frames: [ 1  0  7 ]
 *   Page Z replaces page A (not used for longest time). Frames: [ 2  0  7 ]
 * 
 * Summary format:
 *   Optimal Summary
 *   =================
 *   Reference String: 1 0 7 1 0 2
 *   Page Faults:      Y Y Y N N Y
 *   Total Page Faults: 4
 *   Hit Ratio: 33.33%
 * 
 * @param pages Array containing page reference string
 * @param n Number of page references
 * @param frames Number of available page frames
 */
void optimal(int pages[], int n, int frames);

#endif // PAGE_REPLACEMENT_H
