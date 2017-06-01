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


    im->flipVertically();
    
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


void drawPathF(std::stringstream &ss, potrace_state_t & traced_image)
{
    potrace_curve_t * curve = &traced_image.plist->curve;
    Utils::svg_move_to(ss, curve->c[curve->n-1][2]);
    for(int i=0;i<curve->n;i++)
    {
        if(curve->tag[i] == POTRACE_CURVETO) 
        {
            Utils::svg_curve_to(ss,curve->c[i][0],curve->c[i][1],curve->c[i][2]);
        }
        else if(curve->tag[i] == POTRACE_CORNER)
        {
            Utils::svg_line_to(ss, curve->c[i][1]);
            Utils::svg_line_to(ss, curve->c[i][2]);
        }
    }
}
void drawPathB(std::stringstream &ss, potrace_state_t & traced_image)
{
    potrace_curve_t * curve = &traced_image.plist->curve;
    Utils::svg_move_to(ss, curve->c[curve->n-1][2]);
    //for(int i=0;i<curve->n;i++)
    for(int i=curve->n;i>=0;i--)
    {
        if(i!=0)
        {
            if(curve->tag[i] == POTRACE_CURVETO) 
            {
                Utils::svg_curve_to(ss,curve->c[i][1],curve->c[i][0],curve->c[i-1][2]);
            }
            else if(curve->tag[i] == POTRACE_CORNER)
            {
                Utils::svg_line_to(ss, curve->c[i][1]);
                Utils::svg_line_to(ss, curve->c[i-1][2]);
            }
        }
        else
        {
            if(curve->tag[i] == POTRACE_CURVETO) 
            {
                Utils::svg_curve_to(ss,curve->c[i][0],curve->c[i][1],curve->c[curve->n-1][2]);
            }
            else if(curve->tag[i] == POTRACE_CORNER)
            {
                Utils::svg_line_to(ss, curve->c[i][1]);
                Utils::svg_line_to(ss, curve->c[curve->n-1][2]);
            }
        }
    }
}


void Utils::saveSVG_Single(sf::Image* image)
{
    
    const potrace_bitmap_t * bm = convertToBitmap(image);
    //potrace_bitmap_t bm = 
    potrace_param_s * set = potrace_param_default();
    //potrace_state_s derp(set,potrace_state_s(set,bm));
    potrace_state_t * traced_image = potrace_trace(set,bm);
   // potrace_trace(set,bm);
    //potrace_state_s derp()
    int x = image->getSize().x;
    int y = image->getSize().y;

    std::stringstream ss;
    //Write HEAD
    ss  << "<?xml version=\"1.0\" standalone=\"no\"?>" << "\n"
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\"" << "\n"
        << "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << "\n"
        << "<svg version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\"" << "\n";

    //Write stuff
    ss << "width=\"" << x << ".000pt\" ";
    ss << "height=\"" << y << ".000pt\" ";
    ss << "viewBox=\"0 0 " << x << " " << y << "\"" << "\n";
    ss << "preserveAspectRatio=\"xMidYMid meet\">" << "\n";

    //Write Metadata
    ss << "<metadata>" << "\n";
    ss << "Created by ColorTrace, written by Manuel Strey @2017" << "\n";
    ss << "</metadata>" << "\n";

    //Write g transform
    ss << "<g transform=\"translate(" << 0 << "," << y << ") scale(0.100000,-0.100000)\">" << "\n";
    if(traced_image->plist == NULL) return;

    //Write path beginning
    ss << "<path ";
    ss << "style=\"fill:#ebcd7b fill-opacity:1\"" << "\n";
    ss << "d=\"";

    //Go through all pathes
    do
    {       
        std::cout << "HUGE AS SIGN: " << (char)traced_image->plist->sign << std::endl;

        if((char)traced_image->plist->sign == '+')  drawPathF(ss,*traced_image);
        else                                        drawPathB(ss,*traced_image);


        traced_image->plist = traced_image->plist->next;

    }while(traced_image->plist != NULL);
    
    ss << "z\"/>" << "\n";  
    ss << "</g>" << "\n";
    ss << "</svg>" << "\n";

   std::ofstream f;
   f.open("out_svg.svg");
   f << ss.str();
   f.close();
}


void Utils::saveSVG_Multi(std::vector<sf::Image*> &images, std::vector<sf::Color> &colors)
{
    sf::Image * size_image = images[0];
    int x = size_image->getSize().x;
    int y = size_image->getSize().y;

    potrace_param_s * settings = potrace_param_default();
    std::stringstream ss;

    //Write HEAD
    ss  << "<?xml version=\"1.0\" standalone=\"no\"?>" << "\n"
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\"" << "\n"
        << "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << "\n"
        << "<svg version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\"" << "\n";

    //Write stuff
    ss << "width=\"" << x << ".000pt\" ";
    ss << "height=\"" << y << ".000pt\" ";
    ss << "viewBox=\"0 0 " << x << " " << y << "\"" << "\n";
    ss << "preserveAspectRatio=\"xMidYMid meet\">" << "\n";

    //Write Metadata
    ss << "<metadata>" << "\n";
    ss << "Created by ColorTrace written by Manuel Strey" << "\n";
    ss << "</metadata>" << "\n";

    //Write g transform
    ss << "<g transform=\"translate(" << 0 << "," << y << ") scale(0.100000,-0.100000)\">" << "\n";

    for(int i=images.size()-1; i >= 0; i--)
    //for(unsigned int i=0; i < images.size(); i++)
    {
        sf::Image * image = images[i];
        const potrace_bitmap_t * bm = convertToBitmap(image);
        potrace_state_t * traced_image = potrace_trace(settings,bm);

        if(traced_image->plist == NULL) continue;

        //Write path beginning
        ss << "<path ";
        ss << "style=\"fill:" << Utils::toHex(colors[i]) << " fill-opacity:" << (float)colors[i].a/255.f << "\"" << "\n";
        ss << "d=\"";

        //Go through all pathes
        do
        {       
            //std::cout << "HUGE AS SIGN: " << (char)traced_image->plist->sign << std::endl;

            if((char)traced_image->plist->sign == '+')  drawPathF(ss,*traced_image);
            else                                        drawPathF(ss,*traced_image);

            ss << "z " << "\n";
            traced_image->plist = traced_image->plist->next;

        }while(traced_image->plist != NULL);
        
         ss << "\"/>" << "\n"; 
    }
    
    //End of file
    ss << "</g>" << "\n";
    ss << "</svg>" << "\n";

    //Force stringstream down the files throat and scream the generated shit at it until it obeys
    std::ofstream f;
    f.open(options->output_name + ".svg", std::ofstream::out | std::ofstream::trunc);
    f << ss.str();
    f.close();
    std::cout << "Exporting SVG as PNG: " << options->output_name << ".svg" << "\n";
    std::string command_export = std::string("inkscape -z --export-area-page -w " + std::to_string(x*options->scale) + " -h " + std::to_string(y*options->scale) +  " -f " + options->output_name + ".svg" + " --export-png=" + options->output_name + ".png > nul");
    system(command_export.c_str());
}

potrace_dpoint_t Utils::unit(potrace_dpoint_t p)
{
  potrace_dpoint_t q;
  q.x = (long)(std::floor(p.x*10+.5));
  q.y = (long)(std::floor(p.y*10+.5));
  return q;
}

void Utils::svg_move_to(std::stringstream & ss, potrace_dpoint_t p)
{
    potrace_dpoint_t cur = unit(p);
    ss << "M" << cur.x << " " << cur.y << " ";
}

void Utils::svg_line_to(std::stringstream & ss, potrace_dpoint_t p)
{
    potrace_dpoint_t q;
    q = unit(p);
    ss << "L" << q.x << " " << q.y << " ";
}

void Utils::svg_curve_to(std::stringstream & ss, potrace_dpoint_t p1, potrace_dpoint_t p2, potrace_dpoint_t p3)
{
    potrace_dpoint_t q1, q2, q3;

    q1 = unit(p1);
    q2 = unit(p2);
    q3 = unit(p3);

    ss << "C" << q1.x << " " << q1.y << " " << q2.x << " " << q2.y << " " << q3.x << " " << q3.y << " ";
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