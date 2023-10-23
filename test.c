#include <stdio.h>
#include <string.h>

int main()
{
      char *str = "abcd";
      printf("str: %s", str);
      printf("Str length: %d\n", strlen(str));
      for(int i = 0; i < strlen(str); i++) {
            printf("char: %c\n", str[i]);
      }
      str = "efgh";
      printf("Str length: %d\n", strlen(str));
      for (int i = 0; i < strlen(str); i++)
      {
            printf("char: %c\n", str[i]);
      }
      return 0;
}