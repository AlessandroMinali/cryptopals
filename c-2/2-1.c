#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAD 20

char *pkcs_pad(const int block_size, const char *str, const int size) {
  if (size >= block_size) { return NULL; }

  char *ptr = calloc(block_size, 1);
  for(int i = 0; i < size; ++i) {
    ptr[i] = str[i];
  }

  char pad = block_size - size;
  for(int i = size; i < block_size; ++i) {
    ptr[i] = pad;
  }

  return ptr;
}

int main() {
  char *str = "YELLOW SUBMARINE";

  char *pad_str = pkcs_pad(PAD, str, strlen(str));
  for(int i = 0; i < PAD; ++i) {
    if (pad_str[i] > 31) {
      printf("%c", pad_str[i]);
    } else {
      printf("\\x%.2x", pad_str[i]);
    }
  }
  printf("\n");

  return 0;
}
