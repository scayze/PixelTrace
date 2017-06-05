#include "utils.h"
#include <iomanip>
#include <sstream>
#include <bitset>
#include <limits>
#include <stdio.h>
#include <cmath>
#include <string.h>


Settings * options;

std::string Utils::toHex(sf::Color c)
{
	unsigned long hex = ( (c.r & 0xff) << 16) + ((c.g & 0xff) << 8) + (c.b & 0xff);
	std::stringstream ss;
	ss << "#" << std::setfill('0') << std::setw(6) << std::hex << hex;
	return ss.str();
}

void Utils::removeAlpha(sf::Image * i)
{
    for(unsigned int x=0; x < i->getSize().x; x++)
    {
        for(unsigned int y=0; y < i->getSize().y; y++)
        {
            sf::Color c = i->getPixel(x,y);
            if( c.a != 0 && c.a != 255 )
            {
                i->setPixel(x,y,sf::Color(c.r,c.g,c.b,255));
            }
        } 
    }
}

const potrace_bitmap_t * Utils::convertToBitmap(sf::Image * im)
{
    potrace_bitmap_t * bitmap = new potrace_bitmap_t;
    bitmap->w = im->getSize().x;
    bitmap->h = im->getSize().y;
    const unsigned int digit_count =  std::numeric_limits<unsigned long>::digits;
    bitmap->dy = (im->getSize().x + digit_count - 1) / digit_count;
    bitmap->map = new potrace_word[bitmap->h*bitmap->dy];
    //bitmap->map = arr;
    
    for(int h=0; h < bitmap->h; h++)
    {
        for(int i=0; i < bitmap->dy; i++)
        {  
            int word = h*bitmap->dy + i;

            for(unsigned int j=0; j < digit_count; j++)
            {      
                int x = i*digit_count + j; 
                bitmap->map[word] = bitmap->map[word] << 1;

                if(x > bitmap->w) continue;

                sf::Color c = im->getPixel(x,h);
                if(c.a == 255 && c.r==0 && c.b == 0) bitmap->map[word] = bitmap->map[word] + 1;
            } 
            std::bitset<digit_count> o(bitmap->map[word]);
           // std::cout << o;
            
        }
        //std::cout << std::endl;
      
    }

    ///


    return bitmap;
}

Settings * Utils::readSettings(int argc, char* argv[])
{
    options = new Settings();

	for(int i=0; i < argc; i++)
    {
        std::string arg(argv[i]); 
        std::string val;
        if(i == argc-1) val = "";
        else            val = argv[i+1];
        
        if(arg == "-f" || arg == "--file")
        {
            options->input_name = val;
            if(options->output_name == "") {
                size_t lastindex = val.find_last_of("."); 
                std::string rawname = val.substr(0, lastindex); 
                options->output_name = (rawname + "_out");
            }
        }
        else if(arg == "-o" || arg == "--output")
        {
            options->output_name = val;
        }
        else if(arg == "-t" || arg == "--turdsize")
        {
            options->params.turdsize = std::stoi(val);
        }
        else if(arg == "-a" || arg == "--alphamax")
        {
            options->params.alphamax = std::stod(argv[i+1]);
        }
        else if(arg == "-O" || arg == "--opttolerance")
        {
            options->params.opttolerance = std::stod(argv[i+1]); 
        }
        else if(arg == "-z" || arg == "--turnpolicy")
        {
            options->params.alphamax = std::stoi(val);
            if(val == "black") options->params.turnpolicy = POTRACE_TURNPOLICY_BLACK;
            else if(val == "left") options->params.turnpolicy = POTRACE_TURNPOLICY_LEFT;
            else if(val == "majority") options->params.turnpolicy = POTRACE_TURNPOLICY_MAJORITY;
            else if(val == "minority") options->params.turnpolicy = POTRACE_TURNPOLICY_MINORITY;
            else if(val == "random") options->params.turnpolicy = POTRACE_TURNPOLICY_RANDOM;
            else if(val == "right") options->params.turnpolicy = POTRACE_TURNPOLICY_RIGHT;
            else if(val == "white") options->params.turnpolicy = POTRACE_TURNPOLICY_WHITE;
            else options->params.turnpolicy = std::stoi(val);
        }
        else if(arg == "--help")
        {
            std::cout << "ColorTrace v1.0. Written by Manuel Strey"<< std::endl;
            std::cout << "Command-line Options:"<< std::endl;
            std::cout << "--help            : Display different options and parameters"<< std::endl;
            std::cout << "-f, --file        : define input-file (.bmp, .png, .tga, .jpg, .gif, .psd, .hdr, .pic)"<< std::endl;
            std::cout << "-o, --output      : define output-name (without format!)"<< std::endl;
            std::cout << "-t, --turdsize    : vectorization suppresses speckles of up to this size (default 2)"<< std::endl;
            std::cout << "-a, --alphamax    : vectorization corner threshold parameter (default 1)"<< std::endl;
            std::cout << "-O, --opttolerance: curve optimization tolerance (default 0.2)"<< std::endl;
            std::cout << "-z, --turnpolicy  : how to resolve ambiguities in path decomposition (black,white,left,right,majority,minority,random)"<< std::endl;  

            terminate("");
        }
    }

    if(options->input_name == "")
    {
        terminate("ERROR: No input file defined.");
    }

    return options;
}

void Utils::terminate(std::string r)
{
    std::cerr << r << std::endl;
    std::abort();
}

sf::Image * Utils::quantization(sf::Image * im)
{

}