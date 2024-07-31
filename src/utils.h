#pragma once


extern const size_t ALL_DIGIT_COUNT;
extern int VERBOSE;

const char *get_all_digits();
unsigned int get_digit_value(char base);
char to_digit(unsigned int value);
char *reverse(char *str);
const char *drop_leading_zeros(const char *number);
