#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define MAX_U_CHAR ((u_char)(pow(2, sizeof(u_char) * 8) - 1))
// %hhu - unsigned char format specifier

typedef struct bignum {
  bool sign;
  u_long length;
  u_char *segments;
} bignum;

void pprint(bignum *num);
int toInt(const u_char *c);
bignum *get_bignum();
bignum *new_bignum();
char toHex(u_char c);
bignum *mul(bignum *a, bignum *b);
void mul_single_digit(bignum *a, u_char b);
size_t size(bignum *num);
void add_pos(bignum *a, bignum *b);
bignum *shift(bignum *num, u_long n);
bignum *copy(bignum *num);
void free_bignum(bignum *num);

int main() {
  bignum *a = get_bignum();
  bignum *b = get_bignum();
  bignum *res = mul(a, b);
  pprint(res);
  free_bignum(res);
  free_bignum(a);
  free_bignum(b);
  return 0;
}

size_t size(bignum *num) { return sizeof(bignum) + num->length; }

bignum *new_bignum() {
  bignum *res = malloc(sizeof(bignum));
  *res = (bignum){.length = 0, .sign = true, .segments = NULL};
  return res;
}

void free_bignum(bignum *num) {
  free(num->segments);
  free(num);
}

bignum *copy(bignum *num) {
  bignum *res = new_bignum();
  *res = (bignum){.length = num->length,
                  .sign = num->sign,
                  .segments =
                      memcpy(malloc(num->length), num->segments, num->length)};
  return res;
}

void *safe_realloc(void *ptr, size_t size) {
  void *res = realloc(ptr, size);
  if (res == NULL) {
    printf("realloc() failed");
    exit(1);
  }
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
    res->segments = safe_realloc(res->segments, ++res->length);
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

int toInt(const u_char *c) {
  if (isdigit(*c))
    return *c - '0';
  if (*c >= 'a' && *c <= 'f')
    return *c - 'a' + 10;
  if (*c >= 'A' && *c <= 'F')
    return *c - 'A' + 10;
  return -1;
}

char toHex(u_char c) { return c < 10 ? c + '0' : c - 10 + 'A'; }

bignum *mul(bignum *a, bignum *b) {
  bignum *bignums[b->length]; // intermediate results which will be summed later
  for (int i = 0; i < b->length; i++) {
    bignums[i] = copy(a);
    mul_single_digit(bignums[i], b->segments[b->length - 1 - i]);
    if (i > 0) {
      // add trailing zeroes
      bignums[i]->segments =
          realloc(bignums[i]->segments, bignums[i]->length + i);
      memset(bignums[i]->segments + bignums[i]->length, 0, i);
      bignums[i]->length += i;
    }
  }
  for (u_long i = 1; i < b->length; i++) {
    add_pos(bignums[0], bignums[i]);
    free_bignum(bignums[i]);
  }
  return bignums[0];
}

void mul_single_digit(bignum *a, u_char b) {
  int overflow = 0;
  for (int i = a->length - 1; i >= 0; i--) {
    int res = (int)a->segments[i] * (int)b + overflow;
    a->segments[i] = res % (MAX_U_CHAR + 1);
    overflow = res / (MAX_U_CHAR + 1);
  }
  if (overflow > 0) {
    a = shift(a, 1);
    a->segments[0] = overflow;
  }
}

void add_pos(bignum *a, bignum *b) {
  long diff = a->length - b->length;
  bignum *shorter = diff < 0 ? a : b;
  diff = labs(diff);
  shorter = shift(shorter, diff);
  memset(shorter->segments, 0, diff);
  int overflow = 0;
  for (int i = a->length - 1; i >= 0; i--) {
    int res = (int)a->segments[i] + (int)b->segments[i] + overflow;
    a->segments[i] = res % (MAX_U_CHAR + 1);
    overflow = res / (MAX_U_CHAR + 1);
  }
  if (overflow > 0) {
    a = shift(a, 1);
    a->segments[0] = overflow;
  }
}

bignum *shift(bignum *num, u_long n) {
  num->segments = safe_realloc(num->segments, num->length + n);
  for (int i = num->length - 1 + n; i > 0; i--)
    num->segments[i] = num->segments[i - n];
  num->length += n;
  return num;
}
