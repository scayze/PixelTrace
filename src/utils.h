#ifndef Utils_h
#define Utils_h

#include <SFML/Graphics.hpp>
#include <vector>
#include <potracelib.h>
#include <iostream>
#include <fstream>
#include <exception>

struct Settings
{
    float scale = 4.f;
    bool stack = true;
    std::string input_name = "";
    std::string output_name = "";
    potrace_param_s params = *potrace_param_default();
};

class Utils
{
    public:
    static void removeAlpha(sf::Image * i);
    static std::string toHex(sf::Color c);
    static const potrace_bitmap_t * convertToBitmap(sf::Image * i);

    static Settings * readSettings(int argc, char* argv[]);
    static void terminate(std::string reason);

    sf::Image * quantization(sf::Image * im);
};

#endif