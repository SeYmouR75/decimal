#include "s21_decimal_another_functions.h"

#include "s21_decimal_shared.h"
#include "s21_long_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (get_scale(value) > 28) return 1;

  s21_long_decimal long_value = decimal_to_long(value);
  for (int i = 0; i < get_scale(value); i++) {
    s21_long_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}},
                     r = {{0, 0, 0, 0, 0, 0, 0, 0}};
    int_division(long_value, ten, &long_value, &r);
  }

  set_long_scale(&long_value, 0);

  return long_to_decimal(long_value, result) > 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (get_scale(value) > 28) return 1;

  s21_long_decimal long_value = decimal_to_long(value),
                   r = {{0, 0, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < get_scale(value); i++) {
    s21_long_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
    int_division(long_value, ten, &long_value, &r);
  }

  set_long_scale(&long_value, 0);

  if (r.bits[0] >= 5) {
    s21_long_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
    sum_long_decimal(long_value, one, &long_value);
  }

  return long_to_decimal(long_value, result) > 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (get_scale(value) > 28) return 1;

  int flag = 0;

  s21_long_decimal long_value = decimal_to_long(value);
  for (int i = 0; i < get_scale(value); i++) {
    s21_long_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}},
                     r = {{0, 0, 0, 0, 0, 0, 0, 0}};
    int_division(long_value, ten, &long_value, &r);
    if (r.bits[0] > 0) flag = 1;
  }

  set_long_scale(&long_value, 0);

  if (flag && get_sign(value)) {
    s21_long_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
    sum_long_decimal(long_value, one, &long_value);
  }

  return long_to_decimal(long_value, result) > 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (get_scale(value) > 28) return 1;

  *result = value;
  set_sign(result, get_sign(value) ^ 1);

  return 0;
}