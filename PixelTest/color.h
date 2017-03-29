#define BYTE_RANGED(value) (byte)min(max((uint)(value), (uint) 0x00), (uint) 0xFF)

struct color {
    byte r, g, b;

    color();
    
    color operator= (uint i);
    color(uint i);
    color(byte r, byte g, byte b);

    color operator+ (color a);
    color operator- (color a);
};
        
uint colorToInt(color c)
{
    return ((uint)c.r << 16) + ((uint)c.g << 8) + c.b;
}

color intToColor(uint i)
{
    return color(i >> 16, i >> 8, i);
}

color::color(){
  this->r = 0;
  this->g = 0;
  this->b = 0;
}

color::color(byte r, byte g, byte b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

color::color(uint i)
{
    *this = intToColor(i);
}

color color::operator= (uint i)
{
    return *this = intToColor(i);
}

color color::operator+ (color a)
{
    return {
        BYTE_RANGED((uint)this->r + a.r),
        BYTE_RANGED((uint)this->g + a.g),
        BYTE_RANGED((uint)this->b + a.b)
    };
}

color color::operator- (color a)
{
    return {
        BYTE_RANGED((uint)this->r - a.r),
        BYTE_RANGED((uint)this->g - a.g),
        BYTE_RANGED((uint)this->b - a.b)
    };
}

color operator* (float f, color c)
{
    return{
        BYTE_RANGED(f * c.r),
        BYTE_RANGED(f * c.g),
        BYTE_RANGED(f * c.b)
    };
}

color operator* (color c, float f)
{
    return{
        BYTE_RANGED((float)c.r * f),
        BYTE_RANGED((float)c.g * f),
        BYTE_RANGED((float)c.b * f)
    };
}

color operator/ (color c, float f)
{
    return {
        BYTE_RANGED((float)c.r / f),
        BYTE_RANGED((float)c.g / f),
        BYTE_RANGED((float)c.b / f)
    };
}
