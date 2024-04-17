#include "s21_decimal_shared.h"
#include "s21_long_decimal.h"

int s21_add(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int flag = 0;
  s21_long_decimal long_a = decimal_to_long(a);
  s21_long_decimal long_b = decimal_to_long(b);

  even_scale(&long_a, &long_b);

  if (get_long_sign(long_a) ^ get_long_sign(long_b)) get_inverse(&long_b);

  sum_long_decimal(long_a, long_b, &long_a);

  if (get_bit(223, long_a)) {
    set_long_sign(&long_a, 1 ^ get_long_sign(long_a));
    get_inverse(&long_a);
  }

  flag = long_to_decimal(long_a, result);

  return flag;
}

int s21_sub(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int flag = 0;
  s21_long_decimal long_a = decimal_to_long(a);
  s21_long_decimal long_b = decimal_to_long(b);

  even_scale(&long_a, &long_b);

  if (get_long_sign(long_a) == get_long_sign(long_b)) get_inverse(&long_b);

  sum_long_decimal(long_a, long_b, &long_a);
  if (get_bit(223, long_a)) {
    set_long_sign(&long_a, 1 ^ get_long_sign(long_a));
    get_inverse(&long_a);
  }

  flag = long_to_decimal(long_a, result);

  return flag;
}

int s21_div(s21_decimal a, s21_decimal b, s21_decimal *result) {
  if (is_null(b)) return 3;

  int flag = 0;

  int scale1 = get_scale(a), scale2 = get_scale(b);
  set_scale(&b, 0);
  set_scale(&a, 0);

  int scale = scale1 - scale2;

  s21_long_decimal long_a = decimal_to_long(a);
  s21_long_decimal long_b = decimal_to_long(b);

  s21_long_decimal q, r, res;

  int_division(long_a, long_b, &q, &r);
  res = q;
  long_a = r;
  int len = get_long_len(res);

  while (len < 32) {
    mul_by_ten(&res);
    mul_by_ten(&long_a);
    scale++;
    int_division(long_a, long_b, &q, &r);
    long_a = r;
    sum_long_decimal(res, q, &res);
    len++;
  }

  for (; scale < 0; scale++) {
    mul_by_ten(&res);
  }

  set_long_scale(&res, scale);

  if (get_long_sign(res) == get_long_sign(long_b))
    set_long_sign(&res, 0);
  else
    set_long_sign(&res, 1);

  flag = long_to_decimal(res, result);

  return flag;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;

  int scale1 = get_scale(value_1), scale2 = get_scale(value_2);

  s21_long_decimal long_a = decimal_to_long(value_1);
  s21_long_decimal long_b = decimal_to_long(value_2);

  s21_long_decimal long_res;
  fill_null(&long_res);

  while (!is_long_null(long_b)) {
    if (long_b.bits[0] & 1) sum_long_decimal(long_res, long_a, &long_res);
    long_shift_left(&long_a);
    long_shift_right(&long_b);
  }

  set_long_scale(&long_res, scale1 + scale2);

  if (get_long_sign(long_a) == get_long_sign(long_b))
    set_long_sign(&long_res, 0);
  else
    set_long_sign(&long_res, 1);

  flag = long_to_decimal(long_res, result);

  return flag;
}
