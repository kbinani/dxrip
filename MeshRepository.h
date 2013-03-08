#pragma once

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class MeshRepository {
    public:
        static MeshRepository *Instance();

        void Register(const MeshDescriptor &descriptor, const std::string &contentsHash);

        bool Exists(const MeshDescriptor &descriptor) const;

        std::string GetContentsHash(const MeshDescriptor &descriptor) const;

    private:
        std::map<std::string, std::pair<MeshDescriptor, std::string> > repository;
    };

} } }
