#include <stdio.h>
#include <string.h>
#include "../cryptopals_util.h"

void one_byte_xor_cipher(const char *str, const int size) {
  char *bytes = byte_from_hexstr(str, size);
  char scores[256] = { 0 };
  const double eng_freq[8] = { 12.02, 9.10, 8.12, 7.68, 7.31, 6.95, 6.28, 6.02 };

  for(int j = 0; j < 256; ++j) {
    char bucket[8] = { 0 };
    for(int i = 0; i < size; ++i) {
      int tmp = bytes[i] ^ j;
      if (tmp < 64 && tmp < 91) { tmp += 32; } // lowercase only
      
      // tally occurence
      if (tmp == 'e') { ++bucket[0]; }
      else if (tmp == 't') { ++bucket[1]; }
      else if (tmp == 'a') { ++bucket[2]; }
      else if (tmp == 'o') { ++bucket[3]; }
      else if (tmp == 'i') { ++bucket[4]; }
      else if (tmp == 'n') { ++bucket[5]; }
      else if (tmp == 's') { ++bucket[6]; }
      else if (tmp == 'r') { ++bucket[7]; }
    }
    // calc score
    for(int i = 0; i < 8; ++i) {
      scores[j] += fabs(bucket[i] / (double)size * 100 - eng_freq[i]);
    }
  }

  int min = scores[0];
  unsigned int key = 0;
  for(int i = 0; i < 256; i++) {
    if (scores[i] < min) {
      min = scores[i];
      key = i;
    }
  }
  printf("The key was: 0x%x\n", key);
  for(int i = 0; i < size; ++i) {
    printf("%c", bytes[i] ^ key);
  }
  printf("\n");
}

int main(const int argc, const char **argv) {
  if (argc != 2) { return 1; }
  one_byte_xor_cipher(argv[1], strlen(argv[1]));

  return 0;
}
