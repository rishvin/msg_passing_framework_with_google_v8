#include <stdio.h>

int getnext(int num)
{
    int pos = 0;
    while(num & 1 << pos)
    {
        pos++;
    }
    num = num | (1<< pos);
    if(pos)
    {
        int mask = 1<<pos - 1;
        num = num ^ mask;
    }
    return num;
}

int main()
{
    printf("%d\n", getnext(56));
    printf("%d\n", getnext(0));
    printf("%d\n", getnext(2));
    printf("%d\n", getnext(1));
    return 0;
}
