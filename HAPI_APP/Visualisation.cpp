#include "Visualisation.h"

Visualisation::Visualisation()
{
}

Visualisation::~Visualisation()
{
	for (auto p : sprites_)
		delete p.second;
}

void Visualisation::setPixel(unsigned int x, unsigned int y, HAPI_TColour col)
{
	unsigned int offset = (y * screenWidth_ + x) * 4;
	if (offset > screenWidth_*screenHeight_ * 4)
		return;
	memcpy(screen_ + offset, &col, sizeof(col));
	return;

}

bool Visualisation::initialise()
{
	if (HAPI.Initialise(screenWidth_, screenHeight_))
	{
		screenBox_.Initialise(0, 1280, 0, 720);
		screen_ = HAPI.GetScreenPointer();
		return true;
	}
	else
	{
		return false;
	}
}

void Visualisation::screenWipe(HAPI_TColour col)
{
	for (unsigned int i = 0; i < pixelCount_; i++)
	{
		memcpy(screen_ + i * 4, &col, sizeof(col));
	}
	return;
}

bool Visualisation::createSprite(unsigned int sw, unsigned int sh, std::string name, std::string path)
{
	Sprite *s = new Sprite;
	if (!s->initialise(sw, sh, path))
		return false;

	sprites_[name] = s;
	return true;
}

void Visualisation::render(unsigned int renderType, std::string name, int posX, int posY)
{
	if(renderType == 0)
		sprites_[name]->clipBlit(screen_, screenWidth_, posX, posY, screenBox_);

	if (renderType == 1)
		sprites_[name]->clipAlphaBlit(screen_, screenWidth_, screenHeight_, posX, posY, screenBox_);

	return;
}

unsigned int Visualisation::getSpriteWidth(std::string name)
{
	return sprites_[name]->getWidth();
}

unsigned int Visualisation::getSpriteHeight(std::string name)
{
	return sprites_[name]->getHeight();
}

Rectangle Visualisation::getRectangle(std::string name)
{
	return sprites_[name]->getRectangle();
}
