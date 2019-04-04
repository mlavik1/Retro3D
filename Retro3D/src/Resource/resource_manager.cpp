#include "resource_manager.h"

namespace Retro3D
{
	ResourceManager::ResourceManager()
		: mAsyncLoadThread([&] () { asyncLoadThread_Tick(); })
	{

	}

	Resource* ResourceManager::getCahcedResource(const std::type_index& arg_type, const std::string& arg_path)
	{
		auto typeFindResult = mCachedResources.find(arg_type.hash_code());
		if (typeFindResult != mCachedResources.end())
		{
			const auto caccedResourcesForType = (*typeFindResult).second;
			auto resFindResult = caccedResourcesForType.find(arg_path);
			if (resFindResult != caccedResourcesForType.end())
			{
				return resFindResult->second.Get();
			}
		}

		return nullptr;
	}

	void ResourceManager::OnStart()
	{
#ifndef __EMSCRIPTEN__ // TODO: does emcc support threads yet?
		mAsyncLoadThread.Execute();
#endif
	}

	void ResourceManager::ProcessCompletedAsyncLoads()
	{
		if (mCompletedAsyncResourceLoadRequests.size() > 0)
		{
			mGuard_CompletedAsyncResourceLoadRequests.lock();

			while (mCompletedAsyncResourceLoadRequests.size() > 0)
			{
				const QueuedAsyncResourceLoad& completedRequest = mCompletedAsyncResourceLoadRequests.back();
				completedRequest.resource->ClearObjectFlag(ObjectFlag::AsyncLoading);
				completedRequest.callback();
				mCompletedAsyncResourceLoadRequests.pop_back();
			}

			mGuard_CompletedAsyncResourceLoadRequests.unlock();
		}
	}

	void ResourceManager::asyncLoadThread_Tick()
	{
		// TODO: make this better

		if (mQueuedAsyncResourceLoadRequests.size() > 0)
		{
			mGuard_QueuedAsyncResourceLoadRequests.lock();

			const QueuedAsyncResourceLoad& queuedRequest = mQueuedAsyncResourceLoadRequests.back();
			mQueuedAsyncResourceLoadRequests.pop_back();

			mGuard_QueuedAsyncResourceLoadRequests.unlock();

			// Load the resource
			queuedRequest.resource->LoadResource(queuedRequest.path);

			// Validate resource pointers
			queuedRequest.resource->ClearObjectFlag(ObjectFlag::AsyncLoading);
		}
	}

}

