#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define MAX_CHAR pow(2, sizeof(char) * 8))

typedef struct bignum {
  bool sign;
  u_long length;
  u_char segments[];
} bignum;

void pprint(bignum *num);
int toInt(char *c);
bignum *get_bignum();
bignum *mul(bignum *a, bignum *b);

int main() {
  pprint(get_bignum());
  return 0;
}

bignum *get_bignum() {
  bignum *res = malloc(sizeof(bignum));
  res->length = 0;
  char c = getchar();
  res->sign = c != '-';
  do {
    res->length++;
    res = realloc(res, sizeof(bignum) + sizeof(u_char) * res->length);
    res->segments[res->length - 1] = toInt(&c);
  } while ((c = getchar()) != '\n');
  return res;
}

void pprint(bignum *num) {
  if (!num->sign)
    printf("-");
  for (int i = 0; i < num->length; i++)
    printf("%d ", num->segments[i]);
  printf("\n");
}

int toInt(char *c) {
  if (isdigit(*c))
    return *c - '0';
  else if (*c >= 97 && *c <= 102)
    return *c - 97 + 10; // lowercase hex
  else if (*c >= 65 && *c <= 70)
    return *c - 65 + 10; // uppercase hex
  else
    return -1; // error
}

bignum *mul(bignum *a, bignum *b) {
  bignum *res =
      malloc(sizeof(bignum) + sizeof(u_char) * (a->length + b->length));
  res->length = a->length + b->length;
  res->sign = a->sign ^ b->sign;

  for (int i = b->length - 1; i >= 0; i--) {
    for (int j = a->length - 1; j >= 0; j--) {
    }
  }
}

bignum *mul_single_digit(bignum *a, char b) {
  int overflow = 0;
  for (int i = a->length - 1; i >= 0; i--) {
    int res = (int)a->segments[i] * (int)b;
    if (res > MAX_CHAR) {
      a->segments[i] = MAX_CHAR;
    }
  }
}
