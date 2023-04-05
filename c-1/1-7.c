#include "../cryptopals_util.h"

#define BUF_SIZE 4096

int main() {
  char buf[BUF_SIZE] = { 0 };
  char bytes[BUF_SIZE] = { 0 };
  unsigned char out[BUF_SIZE] = { 0 };

  FILE *f = fopen("7.txt", "rb");
  fread(buf, BUF_SIZE, 1, f);
  fclose(f);

  unsigned char *key = (unsigned char *)"YELLOW SUBMARINE";
  int size = strlen(buf);
  int lines = bytes_from_b64(buf, size, bytes);

  int len = decrypt((unsigned char *)bytes, (size - lines) / 4 * 3, key, out);
  out[len] = '\0';
  printf("%s\n", out);

  return 0;
}
