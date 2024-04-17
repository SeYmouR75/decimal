#include "s21_long_decimal.h"

s21_long_decimal decimal_to_long(s21_decimal a) {
  s21_long_decimal var = {0};

  var.bits[0] = (unsigned)a.bits[0];
  var.bits[1] = (unsigned)a.bits[1];
  var.bits[2] = (unsigned)a.bits[2];
  var.bits[3] = 0;
  var.bits[4] = 0;
  var.bits[5] = 0;
  var.bits[6] = 0;
  var.bits[7] = (unsigned)a.bits[3];

  return var;
}

int get_long_scale(s21_long_decimal num) { return (num.bits[7] >> 16) & 0xFF; }

void set_long_scale(s21_long_decimal *a, int scale) {
  a->bits[7] &= ~(0xff << 16);
  a->bits[7] |= scale << 16;
}

void mul_by_ten(s21_long_decimal *a) {
  unsigned long temp1 = 0, temp2 = 0;
  for (int i = 0; i < 7; i++) {
    temp1 = (unsigned long)a->bits[i] * 10 + temp2;
    temp2 = 0;
    a->bits[i] = temp1 % (1UL << 32);
    temp2 = temp1 / (1UL << 32);
  }
}

void even_scale(s21_long_decimal *a, s21_long_decimal *b) {
  int scale_a = get_long_scale(*a), scale_b = get_long_scale(*b);

  if (scale_a > scale_b) {
    for (int i = 0; i < scale_a - scale_b; i++) mul_by_ten(b);
    set_long_scale(b, scale_a);
  }

  else if (scale_a < scale_b) {
    for (int i = 0; i < scale_b - scale_a; i++) mul_by_ten(a);
    set_long_scale(a, scale_b);
  }
}

int is_long_null(s21_long_decimal num) {
  int flag = 1;
  for (int i = 0; i < 7; i++) {
    if (num.bits[i] != 0) {
      flag = 0;
      break;
    }
  }

  return flag;
}

void sum_long_decimal(s21_long_decimal a, s21_long_decimal b,
                      s21_long_decimal *result) {
  unsigned long temp1 = 0, temp2 = 0;
  for (int i = 0; i < 7; i++) {
    temp1 = (unsigned long)a.bits[i] + b.bits[i] + temp2;
    temp2 = 0;
    result->bits[i] = temp1 % (1UL << 32);
    temp2 = temp1 / (1UL << 32);
  }
}

void get_inverse(s21_long_decimal *a) {
  for (int i = 0; i < 7; i++) {
    a->bits[i] = ~a->bits[i];
  }
  s21_long_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
  sum_long_decimal(*a, one, a);
}

void long_shift_left(s21_long_decimal *a) {
  unsigned t_bit = 0;
  for (int i = 0; i < 7; i++) {
    unsigned temp = a->bits[i] >> 31;
    a->bits[i] <<= 1;
    a->bits[i] |= t_bit;
    t_bit = temp;
  }
}

void long_shift_right(s21_long_decimal *a) {
  unsigned t_bit = 0;
  for (int i = 6; i >= 0; i--) {
    unsigned temp = a->bits[i] & 1;
    a->bits[i] >>= 1;
    a->bits[i] |= t_bit << 31;
    t_bit = temp;
  }
}

int get_bit(int ind, s21_long_decimal a) {
  int i = ind / (sizeof(a.bits[0]) * 8), j = ind % (sizeof(a.bits[0]) * 8);
  return (a.bits[i] & (1U << j));
}

void set_bit(int ind, s21_long_decimal *a) {
  int i = ind / (sizeof(a->bits[0]) * 8), j = ind % (sizeof(a->bits[0]) * 8);
  a->bits[i] |= (1U << j);
}

int equal_long(s21_long_decimal a, s21_long_decimal b) {
  even_scale(&a, &b);
  int flag = 1;
  for (int i = 6; i >= 0; i--) {
    if (a.bits[i] != b.bits[i]) {
      flag = 0;
      break;
    }
  }
  return flag;
}

int greater_or_equal_long(s21_long_decimal a, s21_long_decimal b) {
  even_scale(&a, &b);
  int flag = 1;
  for (int i = 6; i >= 0; i--) {
    if (a.bits[i] > b.bits[i]) {
      break;
    }
    if (a.bits[i] < b.bits[i]) {
      flag = 0;
      break;
    }
  }
  return flag;
}

int greater_long(s21_long_decimal a, s21_long_decimal b) {
  even_scale(&a, &b);
  int flag = 0;
  for (int i = 6; i >= 0; i--) {
    if (a.bits[i] > b.bits[i]) {
      flag = 1;
      break;
    }
    if (a.bits[i] < b.bits[i]) {
      flag = 0;
      break;
    }
  }
  return flag;
}

void fill_null(s21_long_decimal *a) {
  for (int i = 0; i < 8; i++) {
    a->bits[i] = 0;
  }
}

void int_division(s21_long_decimal a, s21_long_decimal b, s21_long_decimal *q,
                  s21_long_decimal *r) {
  if (is_long_null(b)) return;

  fill_null(q);
  fill_null(r);

  s21_long_decimal anti_b = b;
  get_inverse(&anti_b);

  for (int i = 8 * (sizeof(s21_long_decimal) - sizeof(unsigned)) - 1; i >= 0;
       i--) {
    long_shift_left(r);

    if (get_bit(i, a)) set_bit(0, r);

    if (greater_or_equal_long(*r, b)) {
      sum_long_decimal(*r, anti_b, r);
      set_bit(i, q);
    }
  }

  set_long_sign(q, get_long_sign(a));
}

int get_long_len(s21_long_decimal a) {
  int len = 0;
  s21_long_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
  s21_long_decimal q;
  s21_long_decimal r;
  do {
    fill_null(&q);
    fill_null(&r);
    int_division(a, ten, &q, &r);

    a = q;
    len++;

  } while (!is_long_null(a));

  return len;
}

int long_to_decimal(s21_long_decimal long_a, s21_decimal *a) {
  int flag = 0;
  int len = get_long_len(long_a), scale = get_long_scale(long_a);
  if (len - scale > 29) {
    if (long_a.bits[7] & (1U << 31))
      flag = 2;
    else
      flag = 1;
  }

  if (!flag) {
    int scale = get_long_scale(long_a);
    s21_long_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
    s21_long_decimal q = {{0, 0, 0, 0, 0, 0, 0}};
    s21_long_decimal r = {{0, 0, 0, 0, 0, 0, 0}};

    s21_long_decimal reminder = {{0, 0, 0, 0, 0, 0, 0, 0}};
    int reminder_scale = 0;

    for (int i = 29; (i < len || long_a.bits[3] > 0) && scale > 0;
         i++, scale--) {
      int_division(long_a, ten, &q, &r);
      long_a = q;
      for (int i = 0; i < reminder_scale; i++) {
        mul_by_ten(&r);
      }
      sum_long_decimal(reminder, r, &reminder);
      reminder_scale++;
    }

    set_long_scale(&reminder, reminder_scale);
    set_long_scale(&long_a, scale);

    s21_long_decimal zero_five = {{5, 0, 0, 0, 0, 0, 0, 0}};
    set_long_scale(&zero_five, 1);

    if (greater_long(reminder, zero_five)) {
      s21_long_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
      sum_long_decimal(long_a, one, &long_a);
    }

    if (equal_long(reminder, zero_five)) {
      int_division(long_a, ten, &q, &r);
      if (r.bits[0] % 2 != 0) {
        s21_long_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
        sum_long_decimal(long_a, one, &long_a);
      }
    }

    while (scale > 0) {
      int_division(long_a, ten, &q, &r);
      if (!is_long_null(r)) break;
      long_a = q;
      scale--;
    }

    set_long_scale(&long_a, scale);

    if (long_a.bits[3] > 0) {
      if (long_a.bits[7] & (1U << 31))
        flag = 2;
      else
        flag = 1;
    } else {
      a->bits[0] = long_a.bits[0];
      a->bits[1] = long_a.bits[1];
      a->bits[2] = long_a.bits[2];
      a->bits[3] = long_a.bits[7];
    }
  }

  return flag;
}

int get_long_sign(s21_long_decimal a) { return a.bits[7] >> 31; }

void set_long_sign(s21_long_decimal *a, unsigned sign) {
  a->bits[7] &= ~(1U << 31);
  a->bits[7] |= sign << 31;
}