#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iomanip>

std::vector<std::string> files;
std::vector<sf::Image *> images;
std::vector<sf::Color> colors;

std::string input;
int export_dpi = 288;

bool stack = true;

sf::Image pre_result;

std::string toHex(sf::Color c)
{
	unsigned long hex = ( (c.r & 0xff) << 16) + ((c.g & 0xff) << 8) + (c.b & 0xff);
	std::stringstream ss;
	ss << "#" << std::setfill('0') << std::setw(6) << std::hex << hex;
	//ss << std::hex << hex;
	std::cout << "TEST: " << ss.str() << std::endl;
	return ss.str();
}

void combine(sf::Vector2u size)
{
	sf::RenderTexture result;

	result.create(size.x*4,size.y*4,true);
	result.clear(sf::Color(0,0,0,0));

	for(unsigned int i=0; i<files.size(); i++)
	{
		sf::Sprite s;
		sf::Texture t;
		t.loadFromFile( std::string( files[files.size()-1-i]) + ".png");
		s.setTexture(t);
		result.draw(s);
		//std::cout << "drawnFDSFDSFSD " << files[i] << std::endl;
	}	
	result.display();
	sf::Image result_image;
	result_image = result.getTexture().copyToImage();
	result.getTexture().copyToImage().saveToFile(std::string("out_" + input));
	//result_image.saveToFile(std::string("out_" + input));
}

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
				images[current]->setPixel(x,y,colors[current]);
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
	
	std::string name = std::to_string(c.r) + "_" + std::to_string(c.g) + "_" + std::to_string(c.b);
	std::string name_ext = name + ".bmp";
	std::string command_convert = "potrace --svg --flat " + name_ext + " -C \"" + toHex(c) + "\" -o " + name + ".svg";
	std::string command_export = "inkscape -z --export-area-page --export-dpi " + std::to_string(export_dpi) + " -f " + name + ".svg --export-png=" + name + ".png";
	
	files.push_back(std::string(name));
	bool success = image->saveToFile(name_ext);
	
	system(command_convert.c_str());
	system(command_export.c_str());
}

void cleanup()
{
	for(unsigned int i=0; i<files.size();i++)
	{
		system(std::string("rm " + files[i] + ".png").c_str());
		system(std::string("rm " + files[i] + ".bmp").c_str());
		system(std::string("rm " + files[i] + ".svg").c_str());
	}
}

int main(int argc, char* argv[])
{
	std::cout << "argument count " <<  argc << std::endl;	
	for(int i=0; i < argc; i++) std::cout << i << " " << argv[i] << std::endl; 
	input = argv[1];

	sf::Image image;
	image.loadFromFile(input);


	//const sf::Uint8 * data = image.getPixelsPtr();

	//Goes though every pixel and checks if a layer has been created. if not, create one
	sf::Clock counter;
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

	combine(image.getSize());
	cleanup();
	std::cout << counter.getElapsedTime().asSeconds();
	return 0;
}




