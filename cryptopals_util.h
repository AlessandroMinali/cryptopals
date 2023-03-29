#include <stdlib.h>
#include <math.h>

const char *bit_rep[16] = {
  [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
  [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
  [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
  [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void print_byte(char byte) {
  printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

char *bin_from_hexstr(const char *str, const unsigned int size) {
  char *bin = calloc(size + 1, 1);

  for(int i = 0; i <= (int)size; ++i) {
    bin[i] = str[i];
    if (str[i] >= 97) { // deal with lowercase
      bin[i] -= 87;
    } else if (str[i] >= 65) { // deal with uppercase
      bin[i] -= 55;
    } else if (str[i] < 58) { // deal with nums
      bin[i] -= 48;
    }
  }

  return bin;
}
