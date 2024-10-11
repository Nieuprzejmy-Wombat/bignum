#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define MAX_U_CHAR ((u_char)(pow(2, sizeof(u_char) * 8) - 1))
// %hhu - unsigned char format specifier

typedef struct bignum {
  bool sign;
  u_long length;
  u_char segments[];
} bignum;

u_char *last(bignum *num);
void pprint(bignum *num);
int toInt(u_char *c);
bignum *get_bignum();
bignum *new_bignum();
char toHex(u_char c);
// bignum *mul(bignum *a, bignum *b);
void mul_single_digit(bignum *a, u_char b);

int main() {
  // pprint(get_bignum());
  bignum *a = get_bignum();
  pprint(a);
  u_char b;
  scanf("%hhu", &b);
  mul_single_digit(a, b);
  pprint(a);
  return 0;
}

bignum *new_bignum() {
  bignum *res = malloc(sizeof(bignum));
  *res = (bignum){.length = 0, .sign = true};
  return res;
}

// currently least significant at the end
bignum *get_bignum() {
  bignum *res = new_bignum();
  u_char c = getchar();
  if (c == '-') {
    res->sign = false;
    c = getchar();
  }
  u_char d = getchar();
  do {
    res = realloc(res, sizeof(bignum) + sizeof(u_char) * ++res->length);
    res->segments[res->length - 1] =
        toInt(&d) == -1 ? toInt(&c) : toInt(&c) * 16 + toInt(&d);
  } while ((c = getchar()) != '\n' && (d = getchar()) != '\n');
  return res;
}

void pprint(bignum *num) {
  if (!num->sign)
    printf("-");
  for (u_long i = 0; i < num->length; i++)
    printf("%c%c ", toHex((num->segments[i] & 0xF0) / 16),
           toHex(num->segments[i] & 0x0F));
  printf("\n");
}

int toInt(u_char *c) {
  if (isdigit(*c))
    return *c - '0';
  else if (*c >= 97 && *c <= 102)
    return *c - 97 + 10; // lowercase hex
  else if (*c >= 65 && *c <= 70)
    return *c - 65 + 10; // uppercase hex
  else
    return -1; // error
}
char toHex(u_char c) { return c < 10 ? c + '0' : c - 10 + 65; }

bignum *mul(bignum *a, bignum *b) { return a; }

void mul_single_digit(bignum *a, u_char b) {
  int overflow = 0;
  for (int i = a->length - 1; i >= 0; i--) {
    int res = (int)a->segments[i] * (int)b + overflow;
    if (res > MAX_U_CHAR) {
      a->segments[i] = res % (MAX_U_CHAR + 1);
      overflow = (res - a->segments[i]) / (MAX_U_CHAR + 1);
    } else
      a->segments[i] = res;
  }
  if (overflow > 0) {
    // move the array 1 index to the right to make room for overflow
    a = realloc(a, sizeof(bignum) + sizeof(u_char) * ++a->length);
    memcpy(a->segments + sizeof(u_char), a->segments,
           (a->length - 1) * sizeof(u_char));
    a->segments[0] = overflow;
  }
  return;
}

u_char *last(bignum *num) { return &num->segments[num->length - 1]; }
