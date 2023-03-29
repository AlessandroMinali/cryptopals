#include <stdio.h>
#include <string.h>
#include "../cryptopals_util.h"

char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

char *xor_hexstr(const char* hex1, const int size1, const char* hex2, const int size2) {
  char *bin1 = bin_from_hexstr(hex1, size1);
  char *bin2 = bin_from_hexstr(hex2, size2);
  char *xor = NULL;

  for(int i = 0; i < size1; ++i) {
    xor[i] = bin1[i] ^ bin2[i];
    printf("%c", hex[(int)xor[i]]);
  }
  printf("\n");

  free(bin2);
  free(bin1);
  return xor;
}

int main(const int argc, const char **argv) {
  if (argc != 3) { exit(1); }
  xor_hexstr(argv[1], strlen(argv[1]), argv[2], strlen(argv[2]));

  return 0;
}
