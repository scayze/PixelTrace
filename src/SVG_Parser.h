#ifndef SVG_Parser_h
#define SVG_Parser_h

#include "utils.h"

class SVG_Parser
{
    public:
    static void init(Settings * s);
    static void save_SVG(std::vector<const potrace_bitmap_t*>& bitmaps, std::vector<Color> &colors);

    static potrace_dpoint_t unit(potrace_dpoint_t p);
    static void draw_path(std::ofstream &ss, potrace_state_t & traced_image);
    static void svg_move_to(std::ofstream & ss, potrace_dpoint_t p);
    static void svg_curve_to(std::ofstream & ss, potrace_dpoint_t p1, potrace_dpoint_t p2, potrace_dpoint_t p3);
    static void svg_line_to(std::ofstream & ss, potrace_dpoint_t p);

};

#endif