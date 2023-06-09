#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <openssl/evp.h>

#define FREQ_CHAR 13
#define CHAR_BYTES 256

const char *bit_rep[16] = {
  [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
  [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
  [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
  [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void print_byte(char byte) {
  printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

const char eng_chars[FREQ_CHAR] = { ' ', 'e', 't', 'a', 'o', 'i', 'n', 's', 'r', 'h', 'd', 'l', 'u' };
const double eng_freqs[FREQ_CHAR] = { 19.18, 12.02, 9.10, 8.12, 7.68, 7.31, 6.95, 6.28, 6.02, 5.92, 4.32, 3.98, 2.88 };

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

char *bytes_from_hexstr(const char *str, const unsigned int size) {
  char *bin = calloc(size / 2 + 1, 1);
  char bin_l, bin_h;

  for(int i = 0, j = 0; i < (int)size; i+=2, ++j) {
    bin_h = str[i];
    if (str[i] >= 97) { // deal with lowercase
      bin_h -= 87;
    } else if (str[i] >= 65) { // deal with uppercase
      bin_h -= 55;
    } else if (str[i] < 58) { // deal with nums
      bin_h -= 48;
    }
    bin_l = str[i + 1];
    if (str[i + 1] >= 97) { // deal with lowercase
      bin_l -= 87;
    } else if (str[i + 1] >= 65) { // deal with uppercase
      bin_l -= 55;
    } else if (str[i + 1] < 58) { // deal with nums
      bin_l -= 48;
    }
    bin[j] = bin_h << 4 | bin_l;
  }

  return bin;
}

char byte_from_b64char(char c) {
  if (c >= 97) { c -= 71; } // deal with lowercase
  else if (c >= 65) { c -= 65; } // deal with uppercase
  else if (c == '+') { c = 62; }
  else if (c == '/') { c = 63; }
  else if (c == '=') { c = 0; }
  else if (c < 58) { c += 4; } // deal with nums

  return c;
}

struct xor_cipher{
  float score;
  char key;
  char *bytes;
};

float english_score(char *str) {
  float score = 0;
  char bucket[FREQ_CHAR] = { 0 };

  for(int i = 0; i < (int)strlen(str); ++i) {
    for(int k = 0; k < FREQ_CHAR; ++k) {
      if (str[i] > 64 && str[i] < 91) { str[i] += 32; } // lowercase only
      if (str[i] == eng_chars[k]) { ++bucket[k]; printf("%c", eng_chars[k]); break; }
    }
  }

  for(int i = 0; i < FREQ_CHAR; ++i) {
    score += fabs(bucket[i] / (double)strlen(str) * 100 - eng_freqs[i]);
  }

  printf(" %f\n", score);
  return score;
}

struct xor_cipher *one_byte_xor_cipher(char *bytes, const int size) {
  float scores[CHAR_BYTES] = { 0 };

  for(int j = 0; j < CHAR_BYTES; ++j) {
    char bucket[FREQ_CHAR] = { 0 };
    for(int i = 0; i < size; ++i) {
      int tmp = bytes[i] ^ j;

      // tally occurence
      if (tmp > 64 && tmp < 91) { tmp += 32; } // force lowercase
      for(int k = 0; k < FREQ_CHAR; ++k) {
        if (tmp == eng_chars[k]) { ++bucket[k]; break; }
      }
    }

    // calc score
    for(int i = 0; i < FREQ_CHAR; ++i) {
      scores[j] += fabs(bucket[i] / (double)size * 100 - eng_freqs[i]);
    }
  }

  float min = scores[0];
  char key = 0;
  for(int i = 1; i < CHAR_BYTES; i++) {
    if (scores[i] < min) {
      min = scores[i];
      key = i;
    }
    // printf("%x:%f:%d\n", i, scores[i], min);
    // for(int j = 0; j < (int)strlen(bytes); ++j) {
    //   printf("%c", bytes[j] ^ i);
    // }
    // printf("\n");
  }

  struct xor_cipher *result = calloc(sizeof(struct xor_cipher), 1);
  result->score = min;
  result->key = key;
  result->bytes = bytes;

  return result;
}

unsigned int hamming(const char *str1, const char *str2, unsigned int len) {
  unsigned int result = 0;

  for(unsigned int i = 0; i < len; ++i) {
    result += __builtin_popcount(str1[i] ^ str2[i]);
  }

  return result;
}

char *every_nth_byte(const char *str, const unsigned int size, const unsigned int n, const unsigned int block) {
  char *result = calloc(size / block, 1);

  for(int i = 0; i < (int)(size / block); ++i) {
    result[i] = str[i * block + n];
  }

  return result;
}

unsigned int bytes_from_b64(char *src, const unsigned int len, char *dst) {
  int lines = 0;
  for(int i = 0, j = 0; i < (int)len; ++i, ++j) {
    if (src[i] == '\n') {
      --j; // if newlines are present then the cursor is allowed to drift ahead of write head
      ++lines;
    } else {
      src[j] = byte_from_b64char(src[i]);
    }
  }

  char *byt_p = dst;
  for (int count = (len - lines) / 4; count > 0; --count, src += 4, byt_p += 3) {
    byt_p[0] = (src[0] << 2) | (src[1] >> 4);
    byt_p[1] = (src[1] << 4) | ((src[2] >> 2) & 0xf);
    byt_p[2] = (src[2] << 6) | src[3];
  }

  return lines;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char  *key, unsigned char *plaintext) {
  EVP_CIPHER_CTX *ctx;
  int len;
  int plaintext_len;

  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
  EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
  plaintext_len = len;
  EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
  plaintext_len += len;
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}
