#include <string>
#include "utils.h"


std::vector<sf::Image *> images;
std::vector<sf::Color> colors;

Settings * settings;


bool stack = true;
bool noAlpha = false;

void stackImage()
{
	if(images.size()<=1) return;

	unsigned int current = images.size()-1;
	unsigned int last = images.size()-2;

	for(unsigned int x = 0; x < images[last]->getSize().x; x++)
	{
		for(unsigned int y = 0; y < images[last]->getSize().y; y++)
		{
			if(images[last]->getPixel(x,y) != sf::Color(255,255,255,255))
			{
				images[current]->setPixel(x,y,sf::Color(0,0,0,255));//colors[current]);
			}
		}	
	}
}

void createImage(sf::Image * i, sf::Color c)
{
	sf::Image * image = new sf::Image();
	image->create(i->getSize().x,i->getSize().y,sf::Color(255,255,255,255));
	for(unsigned int x = 0; x<image->getSize().x; x++)
	{
		for(unsigned int y = 0; y<image->getSize().y; y++)
		{
			if(i->getPixel(x,y) == c)
			{
				image->setPixel(x,y,sf::Color(0,0,0,255));
			}
		}	
	}

	images.push_back(image);
	if(stack) stackImage();
}

int main(int argc, char* argv[])
{
	sf::Clock counter;
	sf::Image image;
	
	settings = Utils::readSettings(argc,argv);
	image.loadFromFile(settings->input_name);
	//Utils::saveSVG_Single(&image);

	//Goes though every pixel and checks if a layer has been created. if not, create one
	std::cout << "Vectorizing " << settings->input_name << "\n";
	for(int x = 0; x<image.getSize().x; x++)
	{
		for(int y = 0; y<image.getSize().y; y++)
		{
			sf::Color color = image.getPixel(x,y);
			bool layer_exists = false || color.a == 0;	
			for(unsigned int i = 0; i<colors.size(); i++)
			{
				if(color == colors[i]) layer_exists = true;
			}	
			if(!layer_exists)
			{
				colors.push_back(color);
				createImage(&image, color);
			}
		}	
	}

	Utils::saveSVG_Multi(images,colors);

	std::cout << "Processed in: " << counter.getElapsedTime().asSeconds() << "s\n";
	return 0;
}




