#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include "../cryptopals_util.h"

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char  *key, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
  */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

#define BUF_SIZE 4096
int main (void)
{
  char buf[BUF_SIZE] = { 0 };
  char bytes[BUF_SIZE] = { 0 };
  unsigned char out[BUF_SIZE] = { 0 };

  FILE *f = fopen("7.txt", "rb");
  fread(buf, BUF_SIZE, 1, f);
  fclose(f);

  unsigned char *key = (unsigned char *)"YELLOW SUBMARINE";

  int size = strlen(buf);
  int lines = bytes_from_b64(buf, size, bytes);


  /* Decrypt the ciphertext */
  int decryptedtext_len = decrypt((unsigned char *)bytes, (size - lines) / 4 * 3, key, out);

  /* Add a NULL terminator. Expecting printable text */
  out[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  printf("Decrypted text is:\n");
  printf("%s\n", out);

  /* Clean up */
  EVP_cleanup();

  return 0;
}
