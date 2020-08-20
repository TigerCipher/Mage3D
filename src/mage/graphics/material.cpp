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
 * File Name: material.cpp
 * Date File Created: 8/19/2020 at 10:30 PM
 * Author: Matt
 */

#include "mage/graphics/material.h"
#include "mage/core/assetmanager.h"

mage::Material::Material(const char* diffuse, const char* specular, const char* emission, float shininess) :
        m_shininess(shininess)
{
    if (diffuse) m_diffuseTexture = AssetManager::getTexture(diffuse);
    else m_diffuseTexture = AssetManager::getTexture(TEXTURE_DEFAULT_DIFFUSE);
    if (specular) m_specularTexture = AssetManager::getTexture(specular);
    else m_specularTexture = AssetManager::getTexture(TEXTURE_DEFAULT_SPECULAR);
    if (emission) m_emissionTexture = AssetManager::getTexture(emission);
    else m_emissionTexture = AssetManager::getTexture(TEXTURE_DEFAULT_EMISSION);
}
