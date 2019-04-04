#ifndef RETRO3D_CHAIBINDINGS_H
#define RETRO3D_CHAIBINDINGS_H

/*=============================================================================================
Chai Bindings class

Contains ChaiScript bindings for c++ types and functions

==============================================================================================*/

#include "chaiscript/chaiscript.hpp"

namespace Retro3D
{
	class ChaiBindings
	{
	public:
		/** Set up bindings for ChaiScript. */
		static void AddBindings(chaiscript::ChaiScript* arg_chaiscript);
	};
}

#endif
