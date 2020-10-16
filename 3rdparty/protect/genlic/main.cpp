#include <QString>
#include <stdio.h>
#include "lic.h"

int main(int argc, char *argv[])
{
    bool res = false;
    if (argc > 1)
    {
         if (argc > 2)
             res = lic::generate_license(argv[1],argv[2]);
         else
             res = lic::generate_license(argv[1],"");
    }
    printf("Lic file was generated to %s with result=%d\n",argv[1],res);
    return 0;
}
