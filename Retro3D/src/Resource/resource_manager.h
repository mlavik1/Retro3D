#ifndef RETRO3D_RESOURCEMANAGER_H
#define RETRO3D_RESOURCEMANAGER_H

/*=============================================================================================
Resource manager class.

==============================================================================================*/

#include "res_ptr.h"
#include "resource.h"
#include <functional>
#include <vector>
#include <mutex>
#include "Threading/thread.h"
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

namespace Retro3D
{
	struct QueuedAsyncResourceLoad
	{
		Resource* resource;
		std::function<void()> callback;
		std::string path;
	};

	class ResourceManager
	{
	private:
		std::vector<QueuedAsyncResourceLoad> mQueuedAsyncResourceLoadRequests;
		std::vector<QueuedAsyncResourceLoad> mCompletedAsyncResourceLoadRequests;

		std::mutex mGuard_QueuedAsyncResourceLoadRequests;
		std::mutex mGuard_CompletedAsyncResourceLoadRequests;

		void asyncLoadThread_Tick();

		Thread mAsyncLoadThread;
		std::unordered_map<size_t, std::unordered_map<std::string, ResPtr<Resource>>> mCachedResources;

		Resource* getCahcedResource(const std::type_index& arg_type, const std::string& arg_path);

	public:
		ResourceManager();

		/** Called when the engine starts */
		void OnStart();

		/**
		* Process all completed async load requests.
		* This will validate their pointers, and call the lambda passed on to LoadResourceAsync(...)
		*/
		void ProcessCompletedAsyncLoads();
		
		/**
		* Loads a resource on the main thread.
		* @return A resource pointer, that will be valid when loading is done.
		*/
		template <typename T>
		ResPtr<T> LoadResource(const std::string& arg_path)
		{
			const std::type_index& typeIndex = typeid(T);
			T* resObj = (T*)getCahcedResource(typeIndex, arg_path);

			if (resObj == nullptr)
			{
				resObj = new T();

				resObj->LoadResource(arg_path);
				if (mCachedResources.find(typeIndex.hash_code()) == mCachedResources.end())
				{
					mCachedResources[typeIndex.hash_code()] = std::unordered_map<std::string, ResPtr<Resource>>();
				}
				mCachedResources[typeIndex.hash_code()][arg_path] = ResPtr<Resource>(resObj);
			}
			return ResPtr<T>(resObj);
		}

		/**
		* Loads a resource, async.
		* @param  arg_callback  Callback to call when finished loading.
		* @return A resource pointer, that will be valid when loading is done.
		*/
		template <typename T>
		ResPtr<T> LoadResourceAsync(const std::string& arg_path, std::function<void()> arg_callback = {})
		{
#ifdef  __EMSCRIPTEN__ // TODO: does emcc support threads yet?
			ResPtr<T> res = LoadResource<T>(arg_path);
			arg_callback();
			return res;
#endif

			const std::type_index& typeIndex = typeid(T);
			T* resObj = getCahcedResource(typeIndex, arg_path);

			if (resObj == nullptr)
			{
				resObj = new T();

				QueuedAsyncResourceLoad queuedAsyncLoad;
				queuedAsyncLoad.resource = resObj;
				queuedAsyncLoad.callback = arg_callback;

				// TODO: add to a separate queue, and copy over on tick.
				mGuard_QueuedAsyncResourceLoadRequests.lock();
				mQueuedAsyncResourceLoadRequests.push_back(queuedAsyncLoad);
				mGuard_QueuedAsyncResourceLoadRequests.unlock();
			}
			ResPtr<T> resPtr(resObj);

			return resPtr;
		}
		
	};
}

#endif
