#ifndef Utils_h
#define Utils_h

#include <SFML/Graphics.hpp>
#include <vector>
#include <potracelib.h>
#include <iostream>
#include <fstream>

struct Settings
{
    float scale = 4.f;
    bool stack = true;
    std::string input_name;
    std::string output_name = "";
    potrace_param_s params = *potrace_param_default();
};

class Utils
{
    public:
    static void removeAlpha(sf::Image * i);
    static std::string toHex(sf::Color c);
    static const potrace_bitmap_t * convertToBitmap(sf::Image * i);
    static void saveSVG_Single(sf::Image* image);
    static void saveSVG_Multi(std::vector<sf::Image*>& images, std::vector<sf::Color> &colors);

    static potrace_dpoint_t unit(potrace_dpoint_t p);
    static void svg_move_to(std::stringstream & ss, potrace_dpoint_t p);
    static void svg_curve_to(std::stringstream & ss, potrace_dpoint_t p1, potrace_dpoint_t p2, potrace_dpoint_t p3);
    static void svg_line_to(std::stringstream & ss, potrace_dpoint_t p);

    static Settings * readSettings(int argc, char* argv[]);
};

#endif