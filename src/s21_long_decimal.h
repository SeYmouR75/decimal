#include "s21_decimal.h"

typedef struct {
  unsigned int bits[8];
} s21_long_decimal;

s21_long_decimal decimal_to_long(s21_decimal a);

int get_long_scale(s21_long_decimal num);
void set_long_scale(s21_long_decimal *a, int scale);
int is_long_null(s21_long_decimal num);
void mul_by_ten(s21_long_decimal *a);
void even_scale(s21_long_decimal *a, s21_long_decimal *b);
void sum_long_decimal(s21_long_decimal a, s21_long_decimal b,
                      s21_long_decimal *result);
void get_inverse(s21_long_decimal *a);
void long_shift_left(s21_long_decimal *a);
void long_shift_right(s21_long_decimal *a);
int get_bit(int ind, s21_long_decimal a);
void set_bit(int ind, s21_long_decimal *a);
int greater_or_equal_long(s21_long_decimal a, s21_long_decimal b);
void fill_null(s21_long_decimal *a);
void int_division(s21_long_decimal a, s21_long_decimal b, s21_long_decimal *q,
                  s21_long_decimal *r);
int get_long_len(s21_long_decimal a);
int long_to_decimal(s21_long_decimal long_a, s21_decimal *a);
int get_long_sign(s21_long_decimal a);
void set_long_sign(s21_long_decimal *a, unsigned sign);
void fill_decimal_null(s21_decimal *num);
