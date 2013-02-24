#include "MeshRepository.h"

namespace com { namespace github { namespace kbinani {

    MeshRepository *MeshRepository::Instance() {
        static MeshRepository instance;
        return &instance;
    }

    void MeshRepository::Register(const MeshDescriptor &descriptor) {
        if (!Exists(descriptor)) {
            repository.insert(std::make_pair(descriptor.Hash(), descriptor));
        }
    }

    bool MeshRepository::Exists(const MeshDescriptor &descriptor) {
        return repository.find(descriptor.Hash()) != repository.end();
    }

} } }
