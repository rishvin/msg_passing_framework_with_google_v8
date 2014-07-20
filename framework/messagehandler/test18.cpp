#include <stdio.h>

int process(int N, int M, int i, int j)
{
    int size = j - i + 1;
    int ones = 0;
    int final, pattern;
    while(ones)
    {
        ones = ones << 1 | 1;
     }
     ones = ones <<i;
     ones = ~ones;
     pattern = M;
     pattern = pattern << i;
     final = (N & ones) | pattern;
     return final;
}


int main()

{
   printf("%x",process(0x400, 0x15, 2, 6));
   return 0;
}
    
