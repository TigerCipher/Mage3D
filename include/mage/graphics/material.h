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
 * File Name: material.h
 * Date File Created: 8/6/2020 at 4:50 PM
 * Author: Matt
 */

#ifndef MAGE3D_MATERIAL_H
#define MAGE3D_MATERIAL_H

#include "mage3d_exported.h"
#include "mage/common.h"
#include "texture.h"
#include "mage/core/assetmanager.h"
#include <string>
#include <utility>

namespace mage
{
    class Material
    {
    public:
        mage3d_EXPORT explicit Material(const char* diffuse = TEXTURE_DEFAULT_DIFFUSE,
                               const char* specular = TEXTURE_DEFAULT_SPECULAR,
                               const char* emission = TEXTURE_DEFAULT_EMISSION,
                               float shininess = 32.0f) :
                m_shininess(shininess){
            if(diffuse) m_diffuseTexture = AssetManager::getTexture(diffuse);
            else m_diffuseTexture = AssetManager::getTexture(TEXTURE_DEFAULT_DIFFUSE);
            if(specular) m_specularTexture = AssetManager::getTexture(specular);
            else m_specularTexture = AssetManager::getTexture(TEXTURE_DEFAULT_SPECULAR);
            if(emission) m_emissionTexture = AssetManager::getTexture(emission);
            else m_emissionTexture = AssetManager::getTexture(TEXTURE_DEFAULT_EMISSION);
        }

        mage3d_EXPORT inline Texture* getDiffuse() { return m_diffuseTexture; }
        mage3d_EXPORT inline Texture* getSpecular() { return m_specularTexture; }
        mage3d_EXPORT inline Texture* getEmission() { return m_emissionTexture; }
        [[nodiscard]] mage3d_EXPORT inline Texture* getDiffuse() const { return m_diffuseTexture; }
        [[nodiscard]] mage3d_EXPORT inline Texture* getSpecular() const { return m_specularTexture; }
        [[nodiscard]] mage3d_EXPORT inline Texture* getEmission() const { return m_emissionTexture; }
        //mage3d_EXPORT inline SharedPtr<Texture>& getDiffuse() { return m_diffuseTexture; }
        //mage3d_EXPORT inline SharedPtr<Texture>& getSpecular() { return m_specularTexture; }
        //mage3d_EXPORT inline SharedPtr<Texture>& getEmission() { return m_emissionTexture; }
        //[[nodiscard]] mage3d_EXPORT inline SharedPtr<Texture> getDiffuse() const { return m_diffuseTexture; }
        //[[nodiscard]] mage3d_EXPORT inline SharedPtr<Texture> getSpecular() const { return m_specularTexture; }
        //[[nodiscard]] mage3d_EXPORT inline SharedPtr<Texture> getEmission() const { return m_emissionTexture; }
        [[nodiscard]] mage3d_EXPORT inline float getShininess() const { return m_shininess; }

    private:
        Texture* m_diffuseTexture;
        Texture* m_specularTexture;
        Texture* m_emissionTexture;
        //SharedPtr<Texture> m_diffuseTexture;
        //SharedPtr<Texture> m_specularTexture;
        //SharedPtr<Texture> m_emissionTexture;
        float m_shininess{};
    };

}

#endif //MAGE3D_MATERIAL_H
