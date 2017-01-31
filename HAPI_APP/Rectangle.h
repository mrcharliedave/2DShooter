#pragma once
#include <string>

class Rectangle
{
public:
	Rectangle() = default;
	Rectangle(const Rectangle &other);
	~Rectangle();

	void Initialise(int l, int r, int t, int b);

	bool Contains(const Rectangle &other) const;
	bool Outside(const Rectangle &other) const;
	void Translate(int posX, int posY);
	void ClipTo(const Rectangle &other);

	unsigned int Width() const { return std::abs(right_ - left_); };
	unsigned int Height() const { return std::abs(bottom_ - top_); };
	int Get(int name) const;

private:
	int left_{ 0 }, right_{ 0 }, top_{ 0 }, bottom_{ 0 };
};

