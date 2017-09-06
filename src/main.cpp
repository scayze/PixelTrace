#include <string>
#include "utils.h"
#include "SVG_Parser.h"
#include "image.h"

Image * image;
std::vector<Color> colors;
std::vector<const potrace_bitmap_t*> bitmaps;

Settings * settings;

void stackImage(Image * i)
{
	int width;
	int height;
	i->get_size(width,height);
	//i->set_pixel(0,1,Color(0,0,0,255));

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			if(i->get_pixel(x,y) != Color(255,255,255,255))
			{
				image->set_pixel(x,y,Color(0,0,0,255));
			}
		}	
	}
}

void createImage(Image * i, Color c)
{
	int width;
	int height;
	i->get_size(width,height);

	Image color_image;
	color_image.create(width,height,Color(255,255,255,255),4);
	

	for(int x = 0; x<width; x++)
	{
		for(int y = 0; y<height; y++)
		{
			if(i->get_pixel(x,y) == c)
			{
				color_image.set_pixel(x,y,Color(0,0,0,255));
			}
		}	
	}

	stackImage(&color_image);
	bitmaps.push_back(Utils::convertToBitmap(image));

	std::vector<Color> fake_colors = colors;
	std::vector<const potrace_bitmap_t*> fake_bitmaps = bitmaps;
	//SVG_Parser::save_SVG(fake_bitmaps,fake_colors);
}

int main(int argc, char* argv[])
{
	Image * input_image;
	
	settings = Utils::readSettings(argc,argv);
	SVG_Parser::init(settings);
	input_image = new Image(settings->input_name);
	int width;
	int height;
	input_image->get_size(width,height);
	input_image->flip();

	image = new Image();
	image->create(width,height,Color(255,255,255,255),4);

	//Goes though every pixel and checks if a layer has been created. if not, create one
	std::cout << "Vectorizing " << settings->input_name << "\n";
	for(int x = 0; x<width; x++)
	{
		for(int y = 0; y<height; y++)
		{
			Color color = input_image->get_pixel(x,y);

			bool layer_exists = false || color.a == 0;	

			for(unsigned int i = 0; i<colors.size(); i++)
			{
				if(color == colors[i]) layer_exists = true;
				
			}	
			if(!layer_exists)
			{
				colors.push_back(color);
				//std::cout << "Color: " << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << std::endl;
				createImage(input_image, color);
			}
			if(colors.size()>255) Utils::terminate("ERROR: Too many colors");
		}	
	}

	SVG_Parser::save_SVG(bitmaps,colors);

	return 0;
}