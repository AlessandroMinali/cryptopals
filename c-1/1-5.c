#include <string.h>
#include <stdio.h>

int main() {
  char *plaintext = "Still, he didn’t think his situation was all bad. Happiness and boredom, abundance and loneliness filled his life with order, as if these emotions had been woven together in a plaid pattern, and as a result, his life felt strangely balanced. To top everything off, he’d made a friend named Emerson. However, while he was intoxicated by this sense of equilibrium, his bank balance lost its equilibrium, which then unraveled the woven balance of his life.";
  char *key = "ICE";
  unsigned int count = 0;

  char c = '\0';
  while((c = *plaintext++)) {
    printf("%02x", c ^ key[count % strlen(key)]);
    ++count;
  }

  return 0;
}
