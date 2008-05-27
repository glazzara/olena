#ifndef COLOR_H
#define COLOR_H
/*
Szymon Rusinkiewicz
Princeton University

Color.h
Random class for encapsulating colors...
*/

#include "Trimesh/Vec.h"
#include <cmath>
#include <algorithm>
using std::fmod;
using std::floor;
using std::min;
using std::max;
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif


class Color {
	private:
		unsigned char c[3]; // red, green, blue, in that order
	public:
		Color(int r = 0, int g = 0, int b = 0)
		{ c[0] = r; c[1] = g; c[2] = b; }
		Color(const unsigned char *rgb)
		{ c[0] = rgb[0]; c[1] = rgb[1]; c[2] = rgb[2]; }
		Color(double r, double g, double b)
		{
			c[0] = (unsigned char) min(max(256.0 * r, 0.0), 255.0);
			c[1] = (unsigned char) min(max(256.0 * g, 0.0), 255.0);
			c[2] = (unsigned char) min(max(256.0 * b, 0.0), 255.0);
		}
		Color(const double *rgb)
		{
			c[0] = (unsigned char) min(max(256.0 * rgb[0], 0.0), 255.0);
			c[1] = (unsigned char) min(max(256.0 * rgb[1], 0.0), 255.0);
			c[2] = (unsigned char) min(max(256.0 * rgb[2], 0.0), 255.0);
		}

		const unsigned char &operator [] (int i) const
		{ return c[i]; }
		unsigned char &operator [] (int i)
		{ return c[i]; }
		operator const unsigned char *() const
		{ return c; }
		operator unsigned char *()
		{ return c; }

		static Color black()
		{ return Color(0, 0, 0); }
		static Color white()
		{ return Color(255, 255, 255); }
		static Color red()
		{ return Color(255, 0, 0); }
		static Color green()
		{ return Color(0, 255, 0); }
		static Color blue()
		{ return Color(0, 0, 255); }
		static Color yellow()
		{ return Color(255, 255, 0); }
		static Color cyan()
		{ return Color(0, 255, 255); }
		static Color magenta()
		{ return Color(255, 0, 255); }
		static Color hsv(double h, double s, double v)
		{
		// From FvD
			if (s <= 0.0)
				return Color(v,v,v);
			h = fmod(h, 2.0 * M_PI);
			if (h < 0.0)
				h += 2.0 * M_PI;
			h /= M_PI / 3.0;
			int i = int(floor(h));
			double f = h - i;
			double p = v * (1.0 - s);
			double q = v * (1.0 - (s*f));
			double t = v * (1.0 - (s*(1.0-f)));
			switch(i) {
				case 0: return Color(v, t, p);
				case 1: return Color(q, v, p);
				case 2: return Color(p, v, t);
				case 3: return Color(p, q, v);
				case 4: return Color(t, p, v);
				default: return Color(v, p, q);
			}
		}
};

static inline bool operator == (const Color &c1, const Color &c2)
{
	return (c1[0] == c2[0]) && (c1[1] == c2[1]) && (c1[2] == c2[2]);
}

static inline bool operator != (const Color &c1, const Color &c2)
{
	return !(c1 == c2);
}


#endif
