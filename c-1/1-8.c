#include <string.h>
#include "../cryptopals_util.h"

#define BUF_SIZE 512
#define BLOCK 16

int main() {
  FILE *f = fopen("8.txt", "rb");

  char buf[BUF_SIZE] = { 0 };
  char *bytes;
  int count = 0;
  int line = 1;
  int max = 0;
  int max_line = 0;

  while(fgets(buf, BUF_SIZE, f)) {
    bytes = bytes_from_hexstr(buf, strlen(buf));

    for(int i = 0; i < (int)strlen(bytes) - 1; i += BLOCK) {
      for(int j = i + BLOCK; j < (int)strlen(bytes) - 1; j += BLOCK) {
        if (hamming(&bytes[i], &bytes[j], BLOCK) == 0 ) {
          count += 1;
        }
      }
    }

    if (count > max) {
      max = count;
      max_line = line;
    }
    count = 0;
    bzero(buf, BUF_SIZE);
    ++line;
  }
  printf("AES-128 ECB detected on line %d\n", max_line);

  return 0;
}
