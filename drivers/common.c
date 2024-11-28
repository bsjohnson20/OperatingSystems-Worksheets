#include "common.h"


size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

/**
 * @brief Converts a number to a string and stores it in a buffer.
 *
 *
 * @param num The number to convert
 * @param buf The buffer to store the string in
 */
extern void convert_num_to_string(int num, char *buf)
{
    int i = 0;
    while (num > 0)
    {
        buf[i++] = num % 10 + '0';
        num /= 10;
    }
    buf[i] = '\0';
    for (int j = 0; j < i / 2; j++)
    {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }
}