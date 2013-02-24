#pragma once

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class MeshRepository {
    public:
        static MeshRepository *Instance();

        void Register(const MeshDescriptor &descriptor);

        bool Exists(const MeshDescriptor &descriptor);

    private:
        std::map<std::string, MeshDescriptor> repository;
    };

} } }
