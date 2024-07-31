#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>

int VERBOSE = 0;
char all_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

const char *get_all_digits() { return all_digits; }
const size_t ALL_DIGIT_COUNT = sizeof(all_digits)-1;

void arithmatoy_free(char *number) { 
  free(number);
   }

char *arithmatoy_add(unsigned int base, const char *lhs, const char *rhs) {
  size_t len_lhs = { 0 };
  size_t len_rhs = { 0 };
  size_t maxlen = { 0 };
  size_t result_index = { 0 };
  char *result = { 0 };
  unsigned int lhs_digit = 0;
  unsigned int rhs_digit = 0;
  unsigned int rest = 0;
  unsigned int sum = 0;
  unsigned int digit = 0;


  if (VERBOSE) {
    fprintf(stderr, "add: entering function\n");
  }

  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  len_lhs = strlen(lhs);
  len_rhs = strlen(rhs);

  maxlen = (len_lhs > len_rhs) ? len_lhs : len_rhs;
  
  result_index = maxlen + 1;
  
  result = (char*)calloc(maxlen + 2, sizeof(char));

  while (len_lhs > 0 || len_rhs > 0 || rest != 0) {
    lhs_digit = (len_lhs > 0 ? get_digit_value(lhs[--len_lhs]) : 0);
    rhs_digit = (len_rhs > 0 ? get_digit_value(rhs[--len_rhs]) : 0);
    sum = lhs_digit + rhs_digit + rest;
    rest = sum / base;
    digit = sum % base;

    result[--result_index] = to_digit(digit);
  }

  return result + result_index;
}

char *arithmatoy_sub(unsigned int base, const char *lhs, const char *rhs) {
  size_t len_lhs = { 0 };
  size_t len_rhs = { 0 };
  size_t maxlen = { 0 };
  char *result = { 0 };
  int rest = 0;
  int result_index = 0;
  unsigned int lhs_digit = { 0 };
  unsigned int rhs_digit = { 0 };
  unsigned int digit = 0;
  int difference = 0;
  if (VERBOSE) {
    fprintf(stderr, "sub: entering function\n");
  }

  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  if (strcmp(lhs, rhs) == 0) {
    return strdup("0");
  } else if (strcmp(rhs, "0") == 0) {
    return strdup(lhs);
  }

  len_lhs = strlen(lhs);
  len_rhs = strlen(rhs);

  if (len_lhs < len_rhs || (len_lhs == len_rhs && strcmp(lhs, rhs) < 0)) {
    return NULL;
  }

  maxlen = len_lhs;
  result = (char*)calloc(maxlen + 1, sizeof(char));
  result_index = maxlen;

  while (len_lhs > 0 || len_rhs > 0) {
    lhs_digit = (len_lhs > 0 ? get_digit_value(lhs[--len_lhs]) : 0);
    rhs_digit = (len_rhs > 0 ? get_digit_value(rhs[--len_rhs]) : 0);
    difference = lhs_digit - rhs_digit - rest + base;
    rest = (difference >= base) ? 0 : 1;
    digit = difference % base;

    result[--result_index] = to_digit(digit);
  }

  while (result[result_index] == 0x30) {
    result_index++;
  }

  return strdup(result + result_index);
}

char *arithmatoy_mul(unsigned int base, const char *lhs, const char *rhs) {
  size_t len_lhs = { 0 };
  size_t len_rhs = { 0 };
  size_t max_len = { 0 };
  unsigned int *result = { 0 };
  unsigned int carry = 0;
  unsigned int digit_lhs = 0;
  unsigned int digit_rhs = 0;
  unsigned int sum = 0;
  char *result_str = { 0 };
  if (VERBOSE) {
    fprintf(stderr, "mul: entering function\n");
  }

  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  if (strcmp(lhs, "0") == 0 || strcmp(rhs, "0") == 0) {
    return strdup("0");
  }

  len_lhs = strlen(lhs);
  len_rhs = strlen(rhs);
  max_len = len_lhs + len_rhs;
  result = (unsigned int *)calloc(max_len, sizeof(unsigned int));

  for (size_t i = 0; i < len_lhs; i++) {
    digit_lhs = get_digit_value(lhs[len_lhs - 1 - i]);
    for (size_t j = 0; j < len_rhs; j++) {
      digit_rhs = get_digit_value(rhs[len_rhs - 1 - j]);
      sum = result[i + j] + digit_lhs * digit_rhs + carry;
      result[i + j] = sum % base;
      carry = sum / base;
    }
    if (carry > 0) {
      result[i + len_rhs] += carry;
    }
    carry = 0;
  }

  while (max_len > 1 && result[max_len - 1] == 0) {
    max_len--;
  }

  result_str = calloc(max_len + 1, sizeof(char));
  for (size_t i = 0; i < max_len; i++) {
    result_str[max_len - 1 - i] = to_digit(result[i]);
  }

  free(result);
  return result_str;
}



unsigned int get_digit_value(char digit) {
  if (digit >= 0x30 && digit <= 0x39) {
    return digit - 0x30;
  }
  if (digit >= 0x61 && digit <= 0x7a) {
    return 10 + (digit - 0x61);
  }
  return -1;
}



const char *drop_leading_zeros(const char *number) {
  while (*number == 0x30) {
    number++;
  }
  return (*number == 0) ? number - 1 : number;
}

char *reverse(char *str) {
  size_t length  = { 0 };
  char tmp = 0;
  length = strlen(str);
  for (size_t i = 0; i < length / 2; ++i) {
    tmp = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = tmp;
  }
  return str;
}

char to_digit(unsigned int value) {
  if (value >= ALL_DIGIT_COUNT) {
    return 0;
  }
  return get_all_digits()[value];
}

