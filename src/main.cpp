#include <string>
#include "utils.h"
#include "SVG_Parser.h"


sf::Image * image = nullptr;
std::vector<sf::Color> colors;
std::vector<const potrace_bitmap_t*> bitmaps;

Settings * settings;


bool stack = true;
bool noAlpha = false;

void stackImage(sf::Image * i)
{
	for(unsigned int x = 0; x < i->getSize().x; x++)
	{
		for(unsigned int y = 0; y < i->getSize().y; y++)
		{
			if(i->getPixel(x,y) != sf::Color(255,255,255,255))
			{
				image->setPixel(x,y,sf::Color(0,0,0,255));//colors[current]);
			}
		}	
	}
}

void createImage(sf::Image * i, sf::Color c)
{
	sf::Image color_image;
	color_image.create(i->getSize().x,i->getSize().y,sf::Color(255,255,255,255));
	for(unsigned int x = 0; x<color_image.getSize().x; x++)
	{
		for(unsigned int y = 0; y<color_image.getSize().y; y++)
		{
			if(i->getPixel(x,y) == c)
			{
				color_image.setPixel(x,y,sf::Color(0,0,0,255));
			}
		}	
	}

	if(stack) stackImage(&color_image);
	bitmaps.push_back(Utils::convertToBitmap(image));
}

int main(int argc, char* argv[])
{
	sf::Clock counter;
	sf::Image input_image;
	
	settings = Utils::readSettings(argc,argv);
	SVG_Parser::init(settings);
	input_image.loadFromFile(settings->input_name);
	//Utils::saveSVG_Single(&image);
	input_image.flipVertically();

	image = new sf::Image();
	image->create(input_image.getSize().x,input_image.getSize().y,sf::Color(255,255,255,255));

	//Goes though every pixel and checks if a layer has been created. if not, create one
	std::cout << "Vectorizing " << settings->input_name << "\n";
	for(int x = 0; x<input_image.getSize().x; x++)
	{
		for(int y = 0; y<input_image.getSize().y; y++)
		{
			
			sf::Color color = input_image.getPixel(x,y);
			bool layer_exists = false || color.a == 0;	
			for(unsigned int i = 0; i<colors.size(); i++)
			{
				if(color == colors[i]) layer_exists = true;
				
			}	
			if(!layer_exists)
			{
				colors.push_back(color);
				createImage(&input_image, color);
			}
		}	
	}

	SVG_Parser::save_SVG(bitmaps,colors);

	std::cout << "Processed in: " << counter.getElapsedTime().asSeconds() << "s\n";
	return 0;
}




