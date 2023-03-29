#include <stdio.h>
#include <string.h>
#include "../cryptopals_util.h"

char b64[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
               'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
               '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'  };

char *b64_from_hexstr(const char *hexstr, const unsigned int size) {
  char *bin = bin_from_hexstr(hexstr, size);
  char *base64 = calloc((int)ceil(size * 4 / 6.0) + 1, 1);

  int j = 0;
  for(int i = 0; i < (int)size; i+=3, j+=2) {
    base64[j] = (bin[i] << 2) | ((i + 1 < (int)size) ? (bin[i + 1] >> 2) : 0);
    base64[j + 1] = ((bin[i + 1] & 0x3) << 4) | ((i + 2 < (int)size) ? bin[i + 2] : 0);
  }

  if (size % 3 == 1) { j--; } // remove trailing A's
  for(int i = 0; i < j; ++i) {
    printf("%c", b64[(int)base64[i]]); // pretty print b64 string
  }
  for(int i = 0; i < (int)(size % 3); ++i) {
    printf("="); // add padding
  }
  printf("\n");

  free(bin);
  return base64;
}

int main(const int argc, const char **argv) {
  if (argc != 2) { exit(1); }
  b64_from_hexstr(argv[1], strlen(argv[1]));

  return 0;
}
