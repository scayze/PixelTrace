#include "SVG_Parser.h"
#include <stdio.h>
#include <cmath>
#include <string.h>


Settings * set;

void SVG_Parser::init(Settings * o)
{
    set = o;
}

void SVG_Parser::draw_path(std::ofstream &ss, potrace_state_t & traced_image)
{
    potrace_curve_t * curve = &traced_image.plist->curve;
    svg_move_to(ss, curve->c[curve->n-1][2]);
    for(int i=0;i<curve->n;i++)
    {
        if(curve->tag[i] == POTRACE_CURVETO) 
        {
            svg_curve_to(ss,curve->c[i][0],curve->c[i][1],curve->c[i][2]);
        }
        else if(curve->tag[i] == POTRACE_CORNER)
        {
            svg_line_to(ss, curve->c[i][1]);
            svg_line_to(ss, curve->c[i][2]);
        }
    }
}


void SVG_Parser::save_SVG(std::vector<const potrace_bitmap_t*> &bitmaps, std::vector<sf::Color> &colors)
{

    const potrace_bitmap_t * size_image = bitmaps[0];
    int x = size_image->w;
    int y = size_image->h;

    std::ofstream ss;
    ss.open(set->output_name + ".svg", std::ofstream::out | std::ofstream::trunc);



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

    for(int i=bitmaps.size()-1; i >= 0; i--)
    //for(unsigned int i=0; i < images.size(); i++)
    {
        const potrace_bitmap_t * bm = bitmaps[i];
        potrace_state_t * traced_image = potrace_trace(&set->params,bm);

        if(traced_image->plist == NULL) continue;

        //Write path beginning
        ss << "<path ";
        ss << "style=\"fill:" << Utils::toHex(colors[i]) << " fill-opacity:" << (float)colors[i].a/255.f << "\"" << "\n";
        ss << "d=\"";

        //Go through all pathes
        do
        {       
            //std::cout << "HUGE AS SIGN: " << (char)traced_image->plist->sign << std::endl;
            draw_path(ss,*traced_image);

            ss << "z " << "\n";
            traced_image->plist = traced_image->plist->next;

        }while(traced_image->plist != NULL);
        potrace_state_free(traced_image);
        //delete bm->map;
        //delete bm;
        //delete traced_image;
         ss << "\"/>" << "\n"; 
    }
    
    //End of file
    ss << "</g>" << "\n";
    ss << "</svg>" << "\n";

    //Force stringstream down the files throat and scream the generated shit at it until it obeys

    ss.close();

    std::cout << "Exporting SVG as PNG: " << set->output_name << ".svg" << "\n";
    std::string command_export = std::string("inkscape -z --export-area-page -w " + std::to_string(x*set->scale) + " -h " + std::to_string(y*set->scale) +  " -f " + set->output_name + ".svg" + " --export-png=" + set->output_name + ".png > nul");
    system(command_export.c_str());
}

potrace_dpoint_t SVG_Parser::unit(potrace_dpoint_t p)
{
  potrace_dpoint_t q;
  q.x = (long)(std::floor(p.x*10+.5));
  q.y = (long)(std::floor(p.y*10+.5));
  return q;
}

void SVG_Parser::svg_move_to(std::ofstream & ss, potrace_dpoint_t p)
{
    potrace_dpoint_t cur = unit(p);
    ss << "M" << cur.x << " " << cur.y << " ";
}

void SVG_Parser::svg_line_to(std::ofstream & ss, potrace_dpoint_t p)
{
    potrace_dpoint_t q;
    q = unit(p);
    ss << "L" << q.x << " " << q.y << " ";
}

void SVG_Parser::svg_curve_to(std::ofstream & ss, potrace_dpoint_t p1, potrace_dpoint_t p2, potrace_dpoint_t p3)
{
    potrace_dpoint_t q1, q2, q3;

    q1 = unit(p1);
    q2 = unit(p2);
    q3 = unit(p3);

    ss << "C" << q1.x << " " << q1.y << " " << q2.x << " " << q2.y << " " << q3.x << " " << q3.y << " ";
}