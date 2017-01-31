#pragma once
#include <HAPI_lib.h>
#include "Rectangle.h"
using namespace HAPISPACE;

class Sprite
{
public:
	Sprite();
	~Sprite();

	bool initialise(unsigned int w, unsigned int h,  std::string path);

	//Blit without alpha
	void blit(BYTE *destination, unsigned int destWidth, int posX, int posY);
	void clipBlit(BYTE *destination, unsigned int destWidth, int posX, int posY, const Rectangle &destBox);

	//Blit with Alpha
	void alphaBlit(BYTE *destination, unsigned int destWidth, unsigned int destHeight, int posX, int posY);
	void clipAlphaBlit(BYTE *destination, unsigned int destWidth, unsigned int destHeight, int posX, int posY, const Rectangle &destBox);

	//Return functions
	unsigned int getWidth() const { return spriteWidth_; };
	unsigned int getHeight() const { return spriteHeight_; };
	Rectangle getRectangle() const { return spriteBox_; };

private:
	BYTE *data_{ nullptr };

	Rectangle spriteBox_;
	unsigned int spriteWidth_;
	unsigned int spriteHeight_;
};

