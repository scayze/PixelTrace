#include "color.h"

Color::Color()
{
    r=0;
    g=0;
    b=0;
    a=0;
}

Color::Color(unsigned char r, unsigned char g,unsigned char b, unsigned char a)
{
    this-> r = r;
    this-> g = g;
    this-> b = b;
    this-> a = a;
}

bool Color::operator==(Color c)
{
    return (c.r == r && c.g == g && c.b == b && c.a == a); 
}

bool Color::operator!=(Color c)
{
    return (c.r != r || c.g != g || c.b != b || c.a != a); 
}

