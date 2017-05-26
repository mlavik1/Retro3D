#ifndef RETRO3D_OBJECT_H
#define RETRO3D_OBJECT_H

namespace Retro3D
{
	class ObjectRefHandle;

	class Object
	{
	private:
		ObjectRefHandle* mObjectRefHandle;

	public:
		Object();
		~Object();

		inline ObjectRefHandle*  GetRefHandle() { return mObjectRefHandle; }
	};
}

#endif
