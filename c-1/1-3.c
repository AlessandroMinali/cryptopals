#include <stdio.h>
#include <string.h>
#include "../cryptopals_util.h"

int main(const int argc, const char **argv) {
  if (argc != 2) { return 1; }
  char *bytes = byte_from_hexstr(argv[1], strlen(argv[1]));
  struct xor_cipher *result = one_byte_xor_cipher(bytes, strlen(bytes));

  printf("The key was 0x%x with a score of %f\n", result->key, result->score);
  for(int i = 0; i < (int)strlen(result->bytes); ++i) {
    printf("%c", result->bytes[i] ^ result->key);
  }
  printf("\n");

  return 0;
}
