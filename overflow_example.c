#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int value = 5;
  char buffer_one[8], buffer_two[8];

  strcpy(buffer_one, "one"); /* Put "one" into buffer_one. */
  strcpy(buffer_two, "two"); /* Put "two" into buffer_two. */

  printf("[BEFORE] buffer_two is at %p and contains \'%s\'\n", buffer_two,
         buffer_two);
  printf("[BEFORE] buffer_one is at %p and contains \'%s\'\n", buffer_one,
         buffer_one);
  printf("[BEFORE] value is at %p and is %d (0x%08x)\n", &value, value, value);
  printf("\n[STRCPY] copying %d bytes into buffer_one\n\n", strlen(argv[1]));

  strcpy(buffer_one, argv[1]); /* Copy first argument into buffer_two. */

  printf("[AFTER] buffer_two is at %p and contains \'%s\'\n", buffer_two,
         buffer_two);
  printf("[AFTER] buffer_one is at %p and contains \'%s\'\n", buffer_one,
         buffer_one);
  printf("[AFTER] value is at %p and is %d (0x%08x)\n", &value, value, value);
}
