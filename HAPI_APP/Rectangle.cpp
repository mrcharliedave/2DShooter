#include "Rectangle.h"
#include <algorithm>

Rectangle::Rectangle(const Rectangle &other)
{
	left_ = other.left_;
	right_ = other.right_;
	top_ = other.top_;
	bottom_ = other.bottom_;
}

Rectangle::~Rectangle()
{
}

void Rectangle::Initialise(int l, int r, int t, int b)
{
	left_ = l;
	right_ = r;
	top_ = t;
	bottom_ = b;
}

bool Rectangle::Contains(const Rectangle & other) const
{
	if(other.top_ >= top_ && other.bottom_ <= bottom_ && other.right_ <= right_ && other.left_ >= left_)
		return true;

	return false;
}

bool Rectangle::Outside(const Rectangle & other) const
{
	if ((left_ > other.right_) || (top_ > other.bottom_) || (right_ < other.left_) || (bottom_ < other.top_))
		return true;

	return false;
}

void Rectangle::Translate(int posX, int posY)
{
	right_ += posX;
	left_ += posX;
	bottom_ += posY;
	top_ += posY;
}

void Rectangle::ClipTo(const Rectangle & other)
{
	if (left_ < other.left_)
		left_ = other.left_;

	if (right_ > other.right_)
		right_ = other.right_;

	if (top_ < other.top_)
		top_ = other.top_;

	if (bottom_ > other.bottom_)
		bottom_ = other.bottom_;

}

int Rectangle::Get(int name) const
{
	if (name == 0)
		return left_;
	if (name == 1)
		return right_;
	if (name == 2)
		return top_;
	if (name == 3)
		return bottom_;
}
