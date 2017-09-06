
#ifndef Image_H
#define Image_H

#include "color.h"
#include <string>

class Image
{
    private:
        int width;
        int height;
        int n;
        unsigned char * data;
    public:
        Image();
        Image(std::string s);
        void create(int w,int h,Color c,int n);
        void load(std::string s);
        Color get_pixel(int x,int y);
        void set_pixel(int x, int y, Color c);
        void get_size(int &w,int &h);
        void flip();
        int get_comp();

};


#endif