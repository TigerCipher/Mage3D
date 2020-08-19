/*
 * Mage3D
 * Copyright (C) 2020 Blue Moon Development. All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: team@bluemoondev.org
 * 
 * File Name: assetmanager.h
 * Date File Created: 8/19/2020 at 1:41 AM
 * Author: Matt
 */

#ifndef MAGE3D_ASSETMANAGER_H
#define MAGE3D_ASSETMANAGER_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "mage/graphics/texture.h"
#include "mage/graphics/shader.h"
//#include "mage/graphics/model.h"
#include <map>
#include <string>


namespace mage
{
    //class Model;
    class AssetManager
    {
    public:
        mage3d_EXPORT static void loadAssets(const char* baseDir);
        mage3d_EXPORT static void loadTextures(const char* baseDir);
        mage3d_EXPORT static void loadShaders(const char* baseDir);
        mage3d_EXPORT static bool addTexture(const std::string& name, const char* textureFile);
        mage3d_EXPORT static bool addShader(const std::string& name, const char* shaderFile);
        mage3d_EXPORT static void destroy();
        mage3d_EXPORT static inline Texture* getTexture(const std::string& name) { return s_textureMap[ name ].get(); }
        //mage3d_EXPORT static inline SharedPtr<Texture>& getTexture(const std::string& name) {return s_textureMap[ name ];}
        mage3d_EXPORT static inline Shader* getShader(const std::string& name) { return s_shaderMap[name].get(); }
    protected:
    private:
        static std::unordered_map<std::string, UniquePtr<Texture>> s_textureMap;
        //static std::unordered_map<std::string, SharedPtr<Texture>> s_textureMap;
        static std::unordered_map<std::string, UniquePtr<Shader>> s_shaderMap;
        //static std::map<std::string, Model> s_modelMap;
    };

}


#endif //MAGE3D_ASSETMANAGER_H
