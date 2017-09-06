#ifndef Utils_h
#define Utils_h

#include <vector>
#include <potracelib.h>
#include <iostream>
#include <fstream>
#include <exception>
#include "image.h"

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
    static void removeAlpha(Image * i);
    static std::string toHex(Color c);
    static const potrace_bitmap_t * convertToBitmap(Image * i);

    static Settings * readSettings(int argc, char* argv[]);
    static void terminate(std::string reason);

    Image * quantization(Image * im);
};

#endif