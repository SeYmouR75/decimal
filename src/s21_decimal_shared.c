#include "s21_decimal_shared.h"

#include "s21_decimal.h"
#include "s21_long_decimal.h"

int decimal_to_str(s21_decimal num, char* str) {
  s21_long_decimal a = decimal_to_long(num);

  int len = 0, point = 0, scale = get_long_scale(a);
  s21_long_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
  s21_long_decimal q;
  s21_long_decimal r;
  do {
    fill_null(&q);
    fill_null(&r);
    int_division(a, ten, &q, &r);

    a = q;
    if (len == scale && scale != 0) {
      point = 1;
      str[len++] = '.';
    }

    str[len++] = r.bits[0] + '0';

  } while (!is_long_null(q));

  if (len <= scale) {
    int i = len;
    for (; i < scale; i++) {
      str[i] = '0';
    }
    str[i] = '.';
    str[i + 1] = '0';
    len = i + 2;
  }

  str[len] = 0;
  if (get_sign(num)) str[len] = '-', str[len + 1] = 0;

  reverse_string(str);

  return len - point;
}

s21_decimal str_to_decimal(char* str) {
  s21_long_decimal long_num;
  fill_null(&long_num);

  if (str[0] == '-') {
    set_long_sign(&long_num, 1);
    str = str + 1;
  }

  int c = 0;
  for (int i = 0; str[i] != 0; i++) {
    if (str[i] == '.') {
      c++;
      continue;
    }

    mul_by_ten(&long_num);
    s21_long_decimal temp;
    fill_null(&temp);
    temp.bits[0] = str[i] - '0';

    sum_long_decimal(long_num, temp, &long_num);

    if (c > 0) c++;
  }

  set_long_scale(&long_num, c == 0 ? c : c - 1);
  s21_decimal num;
  long_to_decimal(long_num, &num);

  return num;
}

void reverse_string(char* str) {
  int length = strlen(str);
  int start = 0;
  int end = length - 1;
  char temp;

  while (start < end) {
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;

    start++;
    end--;
  }
}

int is_null(s21_decimal num) {
  int flag = 0;
  if (num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0) flag = 1;
  return flag;
}

int get_sign(s21_decimal num) { return ((unsigned)num.bits[3] >> 31) & 1; }

void set_scale(s21_decimal* a, int scale) {
  a->bits[3] &= ~(0xff << 16);
  a->bits[3] |= scale << 16;
}

int get_scale(s21_decimal num) { return (num.bits[3] >> 16) & 0xFF; }

void set_sign(s21_decimal* num, unsigned sign) {
  num->bits[3] &= ~(1U << 31);
  num->bits[3] |= sign << 31;
}

void fill_decimal_null(s21_decimal* num) {
  num->bits[0] = 0;
  num->bits[1] = 0;
  num->bits[2] = 0;
  num->bits[3] = 0;
}
