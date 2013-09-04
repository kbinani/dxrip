#include "Main.h"
#include "Context.h"
#include "Config.h"

namespace dxrip { namespace callback {

	namespace {
        static std::string GetModuleFileDirectory(HANDLE dllHandle)
		{
			char fileName[_MAX_PATH + 1] = "";
			GetModuleFileNameA((HMODULE)dllHandle, fileName, _MAX_PATH);
			char driveName[_MAX_DRIVE + 1] = "";
			char directoryName[_MAX_DIR + 1] = "";
			::_splitpath_s(fileName,
						   driveName, _MAX_DRIVE,
						   directoryName, _MAX_DIR,
						   NULL, 0,
						   NULL, 0);
			return std::string(driveName) + std::string("") + std::string(directoryName);
		}
	}

	struct Context::Impl
	{
		Impl()
			: overlay_(nullptr)
			, config_(nullptr)
			, scene_objects_(nullptr)
			, scene_count_(0)
			, device_(nullptr)
		{
			std::string const directory = GetModuleFileDirectory(globalDllModuleHandle);
			config_ = std::make_shared<dxrip::callback::Config>(directory);
			scene_objects_ = std::make_shared<SceneObjectDao>(config_->GetSceneDataDirectory() + "\\db");
		}

		~Impl()
		{}

		Config const& doGetConfig() const
		{
			return *config_.get();
		}

		SceneObjectDao * doGetSceneObjects() const
		{
			return scene_objects_.get();
		}

		void doIncrementSceneCount()
		{
			++scene_count_;
		}

		void doSetOverlay(ID3D9DeviceOverlay * overlay)
		{
			overlay_ = overlay;
		}

		void doSetDevice(LPDIRECT3DDEVICE9 device)
		{
			device_ = device;
		}

		int doGetSceneCount() const
		{
			return scene_count_;
		}

		LPDIRECT3DDEVICE9 doGetDevice() const
		{
			return device_;
		}

		ID3D9DeviceOverlay * doGetOverlay() const
		{
			return overlay_;
		}

	private:
		LPDIRECT3DDEVICE9 device_;
		ID3D9DeviceOverlay * overlay_;
        std::shared_ptr<dxrip::callback::Config> config_;
        std::shared_ptr<SceneObjectDao> scene_objects_;
		int scene_count_;
	};

    Context::Context()
		: impl_(std::make_shared<Impl>())
	{}

    Context * Context::Instance()
	{
        static Context context;
        return &context;
    }

    Config const& Context::GetConfig() const
	{
        return impl_->doGetConfig();
    }

    Context::~Context()
	{}

    SceneObjectDao * Context::GetSceneObjects() const
	{
        return impl_->doGetSceneObjects();
    }

	void Context::IncrementSceneCount()
	{
		impl_->doIncrementSceneCount();
	}

	ID3D9DeviceOverlay * Context::GetOverlay() const
	{
		return impl_->doGetOverlay();
	}

	void Context::SetOverlay(ID3D9DeviceOverlay * overlay)
	{
		impl_->doSetOverlay(overlay);
	}

	LPDIRECT3DDEVICE9 Context::GetDevice() const
	{
		return impl_->doGetDevice();
	}

	void Context::SetDevice(LPDIRECT3DDEVICE9 device)
	{
		impl_->doSetDevice(device);
	}

	int Context::GetSceneCount() const
	{
		return impl_->doGetSceneCount();
	}

} }
