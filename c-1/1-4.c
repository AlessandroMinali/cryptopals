#include <stdio.h>
#include <string.h>
#include "../cryptopals_util.h"

#define BUF_SIZE 512
#define LINES 327

int main() {
  struct xor_cipher *results[LINES];
  char buf[BUF_SIZE];

  FILE *f = fopen("4.txt", "rb");
  for(int i = 0; i < LINES; ++i) {
    fgets(buf, BUF_SIZE, f);
    results[i] = one_byte_xor_cipher(buf, strlen(buf));
  }

  float min = results[0]->score;
  int index = 0;
  for(int i = 1; i < LINES; ++i) {
    if (results[i]->score < min) {
      min = results[i]->score;
      index = i;
    }
  }

  printf("The key was 0x%x with a score of %f on line %d\n", results[index]->key, results[index]->score, index);
  for(int i = 0; i < (int)strlen(results[index]->bytes) - 1; ++i) {
    printf("%c", results[index]->bytes[i] ^ results[index]->key);
  }
  printf("\n");

  return 0;
}
