#pragma once

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class TextureDescriptor {
    public:
        explicit TextureDescriptor(IDirect3DDevice9 *device, IDirect3DBaseTexture9 *texture) {
            this->device = device;
            this->texture = texture;
            clx::sha1 encoder;
            encoder.reset();
            int devicePointer = (int)device;
            int texturePointer = (int)texture;
            encoder.update((const char *)&devicePointer, sizeof(int));
            encoder.update((const char *)&texturePointer, sizeof(int));
            encoder.finish();
            hash = encoder.to_string();
        }

        std::string Hash() const {
            return hash;
        }

        /**
         * Save texture as png file.
         * @param  filePath
         * @return  sha1 hash of file content
         */
        std::string Save(const std::string &filePath) const {
            if (D3DXSaveTextureToFile(filePath.c_str(), D3DXIFF_PNG, texture, NULL) != D3D_OK) {
                return "";
            }
            const size_t size = 1024;
            char data[size];
            clx::sha1 encoder;
            encoder.reset();
            FILE *file = fopen(filePath.c_str(), "rb");
            if (!file) {
                return "";
            }
            while (!feof(file)) {
                size_t actual = fread((void *)data, sizeof(uint8_t), size, file);
                encoder.update(data, actual * sizeof(uint8_t));
            }
            fclose(file);
            encoder.finish();
            return encoder.to_string();
        }

    private:
        TextureDescriptor() {}

    private:
        IDirect3DDevice9 *device;
        IDirect3DBaseTexture9 *texture;
        std::string hash;
    };

} } }