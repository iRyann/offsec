#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  long n = 27657434811745362;
  char s[256];
  
  fgets(s, 256,stdin);
  if (*(long*)s != 27657434811745362) {
    printf("%lu",*(long*)s);
  }

  return 0;
}
