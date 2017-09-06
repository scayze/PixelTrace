#include "image.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <algorithm>
#include <iterator>

Image::Image()
{

}

Image::Image(std::string s)
{
    load(s);
}

void Image::create(int w,int h,Color c,int n)
{
    data = new unsigned char[w*h*n];
    width = w;
    height = h;
    this->n = n;
    for(int i = 0;i < w*h*n; i+=n)
    {
        data[i+0] = c.r;
        data[i+1] = c.g;
        data[i+2] = c.b;
        data[i+3] = c.a;
    }
}

//Vertically flip an image
//Highly inefficient, but is not too performance impacting, so ehhh, who gives a f
void Image::flip()
{
    //Create a vector and fill it with the data
    for(int i = 0; i < width; i++)
    {
        //New color array
        std::vector<Color> vec;
        //Save colors of vertical line in vec
        for(int j=0; j < height; j++)
            vec.push_back(get_pixel(i,j));
        
        //reverse colors
        std::reverse(std::begin(vec),std::end(vec));

        //Set reversed colors to pixels
        for(int j=0; j < height; j++)
            set_pixel(i,j,vec[j]);
    }
}


void Image::set_pixel(int x,int y, Color c)
{
    data[(y*width + x)*n + 0] = c.r;
    data[(y*width + x)*n + 1] = c.g;
    data[(y*width + x)*n + 2] = c.b;
    data[(y*width + x)*n + 3] = c.a;
}

void Image::load(std::string s)
{
    data = stbi_load(s.c_str(),&width,&height,&n,0);
}

Color Image::get_pixel(int x,int y)
{
    Color color;
    color.r = data[(y*width + x)*n + 0];
    color.g = data[(y*width + x)*n + 1];
    color.b = data[(y*width + x)*n + 2];
    color.a = data[(y*width + x)*n + 3];
    return color;
}

void Image::get_size(int& w, int& h)
{
    w = width;
    h = height;
}

int Image::get_comp()
{
    return n;
}