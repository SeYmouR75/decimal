#include "s21_decimal.h"

int compare(s21_decimal fst_value, s21_decimal scnd_value);
int comparison_of_equals(int scale_one_part, int scale_two_part, int fst_len,
                         char *fst_str, char *scnd_str);
int decimal_to_str(s21_decimal num, char *str);
s21_decimal str_to_decimal(char *str);
int get_sign(s21_decimal num);
int get_scale(s21_decimal num);
void set_scale(s21_decimal *a, int scale);
int is_null(s21_decimal num);
void reverse_string(char *str);
void set_sign(s21_decimal *num, unsigned sign);