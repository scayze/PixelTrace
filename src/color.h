#ifndef Color_H
#define Color_H

class Color
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
    unsigned char a;

    Color();
    Color(unsigned char r, unsigned char g,unsigned char b, unsigned char a);
    bool operator==(Color c);
    bool operator!=(Color c);
    
};

#endif