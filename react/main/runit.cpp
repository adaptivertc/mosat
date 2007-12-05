

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
  while (1)
  {
    system("./ntt tetlan \"Tetl&aacute;n\"");
    system("./ntt periferico_sur \"Perif&eacute;rico Sur\"");
    system("./ntt rtu_prueba \"Prueba\"");
    system("./ntt periferico_norte \"Perif&eacute;rico Norte\"");
    system("./ntt juarez2 \"Ju&aacute;rez 2\"");
    sleep(5);
  }
}
