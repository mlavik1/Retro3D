#ifndef RETRO3D_RESOURCE_H
#define RETRO3D_RESOURCE_H

#include "Object/object.h"
#include <string>

namespace Retro3D
{
	class Resource : public Object
	{
	public:
		/** Loads the resource. Implemented in subclasses of Resource */
		virtual bool LoadResource(const std::string& arg_path) = 0;
	};
}

#endif
