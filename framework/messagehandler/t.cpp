#include <iostream>

int f(int a)
{
    std::cout<<"a";
}

int f(int a, int b)
{
    std::cout<<"b";
}

int main()
{
f(2);
return 0;
}
