#pragma once
#include<map>
#include <HAPI_lib.h>
#include "Sprite.h"
#include "Rectangle.h"

using namespace HAPISPACE;

class Visualisation
{

	struct star
	{
		float x, y, z;
	};

	struct pixel
	{
		unsigned int x, y;
	};

public:
	//Initialisation
	Visualisation();
	~Visualisation();
	bool initialise();

	//Screen manipulation
	void screenWipe(HAPI_TColour col);
	void setPixel(unsigned int x, unsigned int y, HAPI_TColour col);

	//Return screen info
	unsigned int getWidth() const { return screenWidth_; };
	unsigned int getHeight() const { return screenHeight_; };

	//Sprite manipulation
	bool createSprite(unsigned int sw, unsigned int sh, std::string name, std::string path);
	void render(unsigned int renderType,std::string name, int posx, int posy);

	//Return sprite info
	unsigned int getSpriteWidth(std::string name);
	unsigned int getSpriteHeight(std::string name);
	Rectangle getRectangle(std::string name);


private:
	unsigned int screenWidth_{ 1280 };
	unsigned int screenHeight_{ 720 };
	unsigned int pixelCount_{ screenHeight_*screenWidth_ };
	const float moveTick_{ 5.f };

	Rectangle screenBox_;
	pixel centre_ { screenWidth_ / 2,screenHeight_ / 2 };
	BYTE *screen_;
	std::map<std::string, Sprite*> sprites_;


};


