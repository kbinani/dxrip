#pragma once

#include "TextureDescriptor.h"

namespace com { namespace github { namespace kbinani {

    class TextureRepository {
    public:
        static TextureRepository *Instance() {
            static TextureRepository instance;
            return &instance;
        }

        void Register(const TextureDescriptor &descriptor, const std::string &contentsHash) {
            if (!Exists(descriptor)) {
                repository.insert(std::make_pair(descriptor.Hash(), std::make_pair(descriptor, contentsHash)));
            }
        }

        bool Exists(const TextureDescriptor &descriptor) const {
            return repository.find(descriptor.Hash()) != repository.end();
        }

        std::string GetContentsHash(const TextureDescriptor &descriptor) const {
            std::pair<TextureDescriptor, std::string> value = repository.at(descriptor.Hash());
            return value.second;
        }

    private:
        std::map<std::string, std::pair<TextureDescriptor, std::string> > repository;
    };

} } }
