#include "s21_decimal_converters.h"

#include <math.h>

#include "s21_decimal.h"
#include "s21_decimal_shared.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  fill_decimal_null(dst);

  if (src < 0) {
    set_sign(dst, 1);
    src = -src;
  }

  dst->bits[0] = src;

  return 0;
}

float get_fexpr(float num, int *exp) {
  char str[100] = {0};
  sprintf(str, "%e", num);

  for (int i = 0; str[i] != 0; i++) {
    if (str[i] == 'e') {
      str[i] = 'k';
      break;
    }
  }

  float mantissa = 0.0;
  sscanf(str, "%fk%d", &mantissa, exp);

  return mantissa;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  fill_decimal_null(dst);
  s21_decimal max_val = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  float temp = 0.0;
  s21_from_decimal_to_float(max_val, &temp);

  if (isinf(src) || isnan(src) || fabs(src) < 1e-28 || fabs(src) > temp)
    return 1;

  s21_long_decimal val = {{0, 0, 0, 0, 0, 0, 0, 0}};

  if (src < 0) set_long_sign(&val, 1), src = -src;

  int exp = 0;
  float mantissa = get_fexpr(src, &exp);
  exp++;

  for (int i = 0; i < 6; i++, mantissa *= 10)
    ;

  val.bits[0] = (int)mantissa;

  if (exp < 0) set_long_scale(&val, -exp);
  if (exp < 7)
    set_long_scale(&val, 7 - exp);

  else {
    for (int i = 7; i < exp; i++) mul_by_ten(&val);
  }

  long_to_decimal(val, dst);

  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_decimal max_val = {{INT_MAX, 0, 0, 0}}, min_val = {{INT_MIN, 0, 0, 0}};
  set_sign(&min_val, 1);
  if (s21_is_greater(src, max_val) || s21_is_less(src, min_val)) return 1;

  s21_decimal temp;
  int flag = s21_truncate(src, &temp);

  if (!flag) {
    *dst = temp.bits[0];
    if (get_sign(temp)) *dst = -(*dst);
  }

  return flag;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double temp = 0;

  s21_long_decimal long_src = decimal_to_long(src);
  ;

  for (int i = 0; i < 96; i++) {
    if (get_bit(i, long_src)) temp += pow(2.0, (double)i);
  }

  int scale = get_scale(src);
  for (int i = 0; i < scale; i++) {
    temp /= 10;
  }

  if (get_sign(src)) temp = -temp;

  *dst = (float)temp;

  return 0;
}