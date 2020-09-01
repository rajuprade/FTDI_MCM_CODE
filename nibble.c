#include<stdio.h>
int main()
{

    unsigned char a,b;
    a= 0x7f;
     printf ("0x%2X\n",a);
    b=a;
    a = a<<4;
   b = b>>4;
   a = a | b;
   printf ("after nibble changing 0x%2X\n",a);
   return 0;
}