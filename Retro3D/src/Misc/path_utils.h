#ifndef RETRO3D_PATHUTILS_H
#define RETRO3D_PATHUTILS_H

#include <string>

namespace Retro3D
{
    class PathUtils
    {
    public:
        static std::string CombinePaths(const std::string& a, const std::string& b);
    };
}

#endif
