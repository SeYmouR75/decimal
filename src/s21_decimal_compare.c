#include "s21_decimal.h"
#include "s21_decimal_shared.h"
#include "s21_long_decimal.h"

int s21_is_greater(s21_decimal fst_value, s21_decimal scnd_value) {
  return compare(fst_value, scnd_value) == 1 ? 1 : 0;
}

int s21_is_greater_or_equal(s21_decimal fst_value, s21_decimal scnd_value) {
  return ((compare(fst_value, scnd_value) == 1) ||
          (compare(fst_value, scnd_value) == 0))
             ? 1
             : 0;
}

int s21_is_less(s21_decimal fst_value, s21_decimal scnd_value) {
  return compare(fst_value, scnd_value) == -1 ? 1 : 0;
}

int s21_is_less_or_equal(s21_decimal fst_value, s21_decimal scnd_value) {
  return ((compare(fst_value, scnd_value) == -1) ||
          (compare(fst_value, scnd_value) == 0))
             ? 1
             : 0;
}

int s21_is_equal(s21_decimal fst_value, s21_decimal scnd_value) {
  return compare(fst_value, scnd_value) == 0 ? 1 : 0;
}

int s21_is_not_equal(s21_decimal fst_value, s21_decimal scnd_value) {
  return compare(fst_value, scnd_value) != 0 ? 1 : 0;
}

int compare(s21_decimal fst_value, s21_decimal scnd_value) {
  int retval = 0;
  int sign_fstvalue = get_sign(fst_value),
      sign_scndvalue = get_sign(scnd_value);

  if (sign_fstvalue < sign_scndvalue) {
    retval = 1;
  }

  if (sign_fstvalue > sign_scndvalue) {
    retval = -1;
  }

  s21_long_decimal a = decimal_to_long(fst_value),
                   b = decimal_to_long(scnd_value);
  even_scale(&a, &b);

  for (int i = 6; i >= 0 && retval == 0; i--) {
    if (a.bits[i] > b.bits[i]) {
      retval = sign_fstvalue ? -1 : 1;
      break;
    }

    else if (a.bits[i] < b.bits[i]) {
      retval = sign_fstvalue ? 1 : -1;
      break;
    }
  }

  return retval;
}