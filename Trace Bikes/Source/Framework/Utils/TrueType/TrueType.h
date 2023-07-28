#ifndef GameDev2D_TrueType_h
#define GameDev2D_TrueType_h

#include <string>


namespace GameDev2D
{
    //Forward declaration
    class Font;

    class TrueType
    {
    public:
        static void Rasterize(const std::string& filename, const std::string& extension, unsigned int size, const std::string& characterSet, Font** font);
    };
}

#endif