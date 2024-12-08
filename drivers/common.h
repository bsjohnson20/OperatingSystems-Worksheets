#ifndef COMMON_H

#include <stdint.h>
#include <stddef.h>
void convert_num_to_string(int num, char *buf);

int sum_of_three(int arg1, int arg2, int arg3);
int mult_of_three(int arg1, int arg2, int arg3);
int sub_num(int arg1, int arg2);
void asm_display_num(int num);

#define COMMON_H

#endif // !COMMON_H