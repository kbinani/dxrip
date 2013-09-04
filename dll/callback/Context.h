#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Main.h"
#include "Config.h"
#include "dao/SceneObjectDao.h"

namespace dxrip { namespace callback {

	class Config;

    class Context
	{
    public:
        ~Context();

        static Context * Instance();

		void SetDevice(LPDIRECT3DDEVICE9 device);
        LPDIRECT3DDEVICE9 GetDevice() const;
		void SetOverlay(ID3D9DeviceOverlay * overlay);
		ID3D9DeviceOverlay * GetOverlay() const;
		int GetSceneCount() const;
        Config const& GetConfig() const;
        SceneObjectDao * GetSceneObjects() const;
		void IncrementSceneCount();

	private:
		struct Impl;
		std::shared_ptr<Impl> impl_;

        Context();
	};

} }

#endif
