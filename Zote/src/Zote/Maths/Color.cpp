#include "Color.h"

namespace Zote 
{
	Color::Color()
		: r(0), g(0), b(0), a(0) {}

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a) {}

	Color Color::Normalize(const Color& color)
	{
		Color c;
		c.r = color.r / 255;
		c.g = color.g / 255;
		c.b = color.b / 255;
		c.a = 1;
		return c;
	}

	Color Color::Random()
	{
		Color c;
		c.r = RandomFloat(0, 1);
		c.g = RandomFloat(0, 1);
		c.b = RandomFloat(0, 1);
		c.a = 1;
		return c;
	}

	bool Color::operator==(Color color)
	{
		return r == color.r &&  g == color.g && b == color.b && a == color.a;
	}

	const Color Color::grey(108.0f, 122.0f, 137.0f, 1.0f);
	const Color Color::red(1, 0, 0, 1);
	const Color Color::green(0, 1, 0, 1);
	const Color Color::blue(0, 0, 1, 1);
}