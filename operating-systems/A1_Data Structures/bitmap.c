// Bitmap Data Structure - Skeleton file
// See ds_header.h for structure and prototype definitions
// Used in: page frame tracking, CPU masks, process scheduling

#include "ds_header.h"

// Global bitmap storage (defined in header file)
unsigned char bitmap_data[MAX_BITMAP_BYTES] = {0};
int current_bitmap_size = 0;

/******************************************************************
    ⚠️PROVIDED IMPLEMENTATION -Students dont implement this!
******************************************************************/
/**
 * init_bitmap - Initialize bitmap with specified size
 * @param size: Number of bits in the bitmap (1 to MAX_BITMAP_BITS)
 *
 * PROVIDED IMPLEMENTATION - No need to implement this function
 * This function is provided to help students focus on core bit manipulation concepts
 */
void init_bitmap(int size)
{
    // check size bounds
    if (size <= 0 || size > MAX_BITMAP_BITS)
    {
        return;
    }

    current_bitmap_size = size;

    // calculate the bytes needed for 'size' bits
    // 8 bits needs 1 byte, 10 bits needs 2 bytes
    int bytesNeeded = (size + 7) / 8;

    // clear bytes
    for (int i = 0; i < bytesNeeded; i++)
    {
        bitmap_data[i] = 0; // init to zeros
    }
}

/******************************************************************
  👍STUDENT IMPLEMENTATION -Students implement these 8 functions
******************************************************************/

//------------------------------------------------------------------//

/**
 * set_bit - Set bit n to 1
 * @param n: Bit index to set (0-based)
 */
void set_bit(int n)
{
    // TODO: Validate index and set the bit using bitwise OR
    // Use: bitmap_data[n/8] |= (1 << (n%8))

    // check bounds
    if (n >= current_bitmap_size || n < 0)
    {
        return;
    }
    // calculate the byte and bit position within the byte
    int bytePos = n / 8;
    int bitPos = n % 8;
    // set the bit
    bitmap_data[bytePos] |= (1 << bitPos);
}

//------------------------------------------------------------------//

/**
 * clear_bit - Clear bit n to 0
 * @param n: Bit index to clear (0-based)
 */
void clear_bit(int n)
{
    // TODO: Validate index and clear the bit using bitwise AND with NOT
    // Use: bitmap_data[n/8] &= ~(1 << (n%8))

    // check bounds
    if (n >= current_bitmap_size || n < 0)
    {
        return;
    }
    // calculate byte and bit position
    int bytePos = n / 8;
    int bitPos = n % 8;
    // clear the bit with bitwise and inverting
    bitmap_data[bytePos] &= ~(1 << bitPos);
}

//------------------------------------------------------------------//

/**
 * test_bit - Test if bit n is set
 * @param n: Bit index to test (0-based)
 * @return: 1 if set, 0 if clear, -1 if invalid index
 */
int test_bit(int n)
{
    // TODO: Validate index and test the bit using bitwise AND
    // Use: (bitmap_data[n/8] & (1 << (n%8))) != 0

    // chcek bounds
    if (n >= current_bitmap_size || n < 0)
    {
        return -1;
    }
    // calc byte and bit positions
    int bytePos = n / 8;
    int bitPos = n % 8;

    // returns 1 if bit is set and 0 if its cleared
    return (bitmap_data[bytePos] & (1 << bitPos)) != 0 ? 1 : 0;
}

//------------------------------------------------------------------//

/**
 * find_first_zero_bit - Find first unset bit
 * @return: Index of first zero bit, or -1 if all bits are set
 */
int find_first_zero_bit(void)
{
    // TODO: Loop through all bits and find the first one that is 0
    // Use test_bit() function to check each bit

    // loop through the bits to find the first zero
    for (int i = 0; i < current_bitmap_size; i++)
    {
        if (test_bit(i) == 0)
        {
            return i;
        }
    }
    return -1;
}

//------------------------------------------------------------------//

/**
 * find_next_set_bit - Find next set bit after position start
 * @param start: Starting position to search from
 * @return: Index of next set bit, or -1 if none found
 */
int find_next_set_bit(int start)
{
    // TODO: Loop from (start+1) to bitmap_size and find first set bit
    // Use test_bit() function to check each

    // loop through to find the first set bit = 1
    for (int i = start + 1; i < current_bitmap_size; i++)
    {
        if (test_bit(i) == 1)
        {
            return i;
        }
    }
    // nothing found
    return -1;
}

//------------------------------------------------------------------//

/**
 * print_bitmap - Print bitmap in binary format (MSB to LSB in each byte)
 * Format: Print only the bits, space between bytes, no newlines
 */
void print_bitmap(void)
{
    // TODO: Loop through bytes and bits to print binary representation
    // Print from MSB to LSB in each byte: bit 7, 6, 5, 4, 3, 2, 1, 0
    // Add space between bytes, only print bits within bitmap size

    // empty case
    if (current_bitmap_size == 0)
    {
        return;
    }

    int bytesNeeded = (current_bitmap_size + 7) / 8;

    // print 8 bits at a time = 1 byte and add spaces
    for (int bytePosition = bytesNeeded - 1; bytePosition >= 0; bytePosition--)
    {

        // print 8 bits
        for (int bitPosition = 7; bitPosition >= 0; bitPosition--)
        {
            // calculate the bits index in the bitmap
            int bitmapPosition = bytePosition * 8 + bitPosition;

            // prints if it exists in the map
            if (bitmapPosition < current_bitmap_size)
            {
                printf("%d", (bitmap_data[bytePosition] >> bitPosition) & 1);
            }
        }
        // spaces between
        if (bytePosition > 0)
        {
            printf(" ");
        }
    }
}

//------------------------------------------------------------------//

/**
 * is_bitmap_empty - Check if all bits are 0
 * @return: 1 if all bits are 0, 0 if any bit is set
 */
int is_bitmap_empty(void)
{
    // TODO: Loop through all bits and check if any is set
    // Use test_bit() or check bytes directly

    // empty case
    if (current_bitmap_size == 0)
    {
        return 1;
    }

    // loop through each bit to check if any are set = 1
    for (int i = 0; i < current_bitmap_size; i++)
    {
        if (test_bit(i) == 1)
        {
            return 0;
        }
    }
    // all zeros = empty
    return 1;
}

//------------------------------------------------------------------//

/**
 * bitmap_size - Get current bitmap size
 * @return: Number of bits in the current bitmap
 */
int bitmap_size(void)
{
    // TODO: Return the current_bitmap_size
    return current_bitmap_size;
}

//------------------------------------------------------------------//