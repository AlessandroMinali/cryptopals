#include <string.h>
#include <stdio.h>

int main() {
  char *plaintext = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  char *key = "ICE";
  unsigned int count = 0;

  char c = '\0';
  while((c = *plaintext++)) {
    printf("%02x", c ^ key[count % strlen(key)]);
    ++count;
  }

  return 0;
}
