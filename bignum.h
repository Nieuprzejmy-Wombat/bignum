#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdbool.h>
#include <sys/types.h>

typedef bool predicate(char c);

typedef struct bignum {
  bool sign;
  u_long length;
  u_char *segments;
} bignum;

void pprint(bignum *num);
int toInt(u_char c);
bignum *get_bignum(char end);
bignum *new_bignum();
char toHex(u_char c);
bignum *mul(bignum *a, bignum *b);
void mul_single_digit(bignum *a, u_char b);
size_t size(bignum *num);
void add_pos(bignum *a, bignum *b);
bignum *shift(bignum *num, u_long n);
bignum *copy(bignum *num);
void free_bignum(bignum *num);
#endif
