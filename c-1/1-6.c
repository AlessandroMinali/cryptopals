#include <stdio.h>
#include "../cryptopals_util.h"

#define BUF_SIZE 4096

char b64[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
               'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
               '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

int main() {
  char buf[BUF_SIZE] = { 0 };
  char bytes[BUF_SIZE] = { 0 };

  FILE *f = fopen("6.txt", "rb");
  fread(buf, BUF_SIZE, 1, f);
  fclose(f);

  int size = strlen(buf);
  int lines = bytes_from_b64(buf, size, bytes);

  int b_count = (size - lines) / 4 * 3;
  unsigned int keysize = 0;
  double distance = 999;
  for(int i = 2; i < 40; ++i) {
    double result = 0;
    for(int j = 0; j < b_count / i; ++j) {
      result += hamming(&bytes[j * i], &bytes[j * i + i], i) / i;
    }
    result /= b_count / i;

    if (result < distance) {
      keysize = i;
      distance = result;
    }
  }

  char key[40] = { 0 };
  for(int i = 0; i < (int)keysize; ++i) {
    key[i] = one_byte_xor_cipher(every_nth_byte(bytes, b_count, i, keysize), b_count / keysize)->key;
  }
  printf("The key is \"%s\"\n", key);
  unsigned int count = 0;
  for(int i = 0; i < b_count; ++i) {
    printf("%c", bytes[i] ^ key[count % strlen(key)]);
    ++count;
  }
  printf("\n");

  return 0;
}
