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
 * File Name: assetmanager.cpp
 * Date File Created: 8/19/2020 at 1:41 AM
 * Author: Matt
 */

#include "mage/core/assetmanager.h"
//#include "mage/graphics/model.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

std::unordered_map<std::string, UniquePtr<mage::Texture>> mage::AssetManager::s_textureMap;
//std::unordered_map<std::string, SharedPtr<mage::Texture>> mage::AssetManager::s_textureMap;
std::unordered_map<std::string, UniquePtr<mage::Shader>> mage::AssetManager::s_shaderMap;
std::unordered_map<std::string, UniquePtr<mage::ModelData>> mage::AssetManager::s_modelMap;
std::unordered_map<std::string, UniquePtr<mage::Material>> mage::AssetManager::s_materialMap;
int mage::AssetManager::s_assetCount;

void textureCallback(file_t* file, void* udata)
{
    if (!file->isFile) return;
    if (strcmp(file->ext, ".png") == 0 || strcmp(file->ext, ".jpg") == 0)
    {
        std::string name(file->path);
        // TODO: Check for type of texture. Assume the naming convention is textureName_type.ext
        // Or maybe maybe some sort of .material file? like json or something pointing to the different textures?
        //i.e bricks_diffuse.png and bricks_specular.png
        if (mage::AssetManager::addTexture(name, file->path))
            *(int*) udata += 1;
    }
}

void shaderCallback(file_t* file, void* udata)
{
    if (!file->isFile) return;
    if (strcmp(file->ext, ".frag") == 0 || strcmp(file->ext, ".vert") == 0)
    {
        std::string name(file->path);
        int i = name.find_last_of('.');
        name = name.substr(0, i);
        if (mage::AssetManager::addShader(name, name.c_str()))
            *(int*) udata += 1;
    }
}

void modelCallback(file_t* file, void* udata)
{
    if (!file->isFile) return;
    if (strcmp(file->ext, ".obj") == 0)
    {
        std::string name(file->path);
        if (mage::AssetManager::addModel(name, file->path))
            *(int*) udata += 1;
    }
}

void mage::AssetManager::loadTextures(const char* baseDir)
{
    PROFILER_SCOPE(1);
    LOG_TRACE("Loading textures from {}", baseDir);
    int n = 0;
    traverse_r(baseDir, textureCallback, &n);
    s_assetCount += n;
    LOG_TRACE("Loaded {} textures", n);
}


void mage::AssetManager::loadShaders(const char* baseDir)
{
    PROFILER_SCOPE(1);
    LOG_TRACE("Loading shaders registered in {}", baseDir);
    int n = 0;
    //traverse_r(baseDir, shaderCallback, &n);
    std::ifstream f(baseDir);
    json j;
    f >> j;

    for (const auto& item : j[ "Shaders" ])
    {
        LOG_INFO("Loading shader program: {}", item[ "id" ].get<std::string>());
        addShader(item[ "id" ].get<std::string>(), item[ "vertex" ].get<std::string>().c_str(),
                  item[ "fragment" ].get<std::string>().c_str());
        n++;
    }
    s_assetCount += n;
    LOG_TRACE("Loaded {} shaders", n);
}

void mage::AssetManager::loadModels(const char* baseDir)
{
    PROFILER_SCOPE(1);
    LOG_TRACE("Loading models from {}", baseDir);
    int n = 0;
    traverse_r(baseDir, modelCallback, &n);
    s_assetCount += n;
    LOG_TRACE("Loaded {} models", n);
}


void mage::AssetManager::loadAssets(const char* baseDir)
{
    // TODO: Fine for now, but for large games, surely not *everything* must be loaded into ram all at once
    // TODO: Perhaps assign IDs to scenes and load assets from respected folders, i.e for scene 1: ./assets/01/textures
    LOG_INFO("Loading assets from {}", baseDir);
    std::string texDir(baseDir);
    texDir += "/textures";
    std::string shaderDir(baseDir);
    shaderDir += "/shaders.json";
    std::string modelDir(baseDir);
    modelDir += "/models";
    loadTextures(texDir.c_str());
    loadShaders(shaderDir.c_str());
    loadModels(modelDir.c_str());
    std::string matDir(baseDir);
    matDir += "/materials.json";
    loadMaterials(matDir.c_str());

    LOG_INFO("{} assets loaded", s_assetCount);
}

bool mage::AssetManager::addTexture(const std::string& name, const char* textureFile)
{
    bool ret = true;
    auto it = s_textureMap.find(name);
    if (it != s_textureMap.end()) ret = false;
    if (ret)
        s_textureMap.insert(std::make_pair(name, createScope<Texture>(textureFile)));
    return ret;
    //s_textureMap.insert(std::make_pair(name, createRef<Texture>(textureFile)));
    //s_textureMap.insert(std::make_pair(name, new Texture(textureFile)));
}

bool mage::AssetManager::addShader(const std::string& name, const char* shaderFile)
{
    bool ret = true;
    auto it = s_shaderMap.find(name);
    if (it != s_shaderMap.end())
        ret = false;
    if (ret)
        s_shaderMap.insert(std::make_pair(name, createScope<Shader>(shaderFile)));
    return ret;
}

bool mage::AssetManager::addShader(const std::string& name, const char* vertexFile, const char* fragFile)
{
    bool ret = true;
    auto it = s_shaderMap.find(name);
    if (it != s_shaderMap.end())
        ret = false;
    if (ret)
        s_shaderMap.insert(std::make_pair(name, createScope<Shader>(vertexFile, fragFile)));
    return ret;
}

bool mage::AssetManager::addModel(const std::string& name, const char* modelFile)
{
    bool ret = true;
    auto it = s_modelMap.find(name);
    if (it != s_modelMap.end())
        ret = false;
    if (ret)
        s_modelMap.insert(std::make_pair(name, createScope<ModelData>(modelFile)));
    return ret;
}

void mage::AssetManager::destroy()
{
    LOG_INFO("Cleaning up loaded assets");
    for (const auto& item : s_textureMap)
    {
        item.second->destroy();
    }
    s_textureMap.clear();

    for (const auto& item : s_shaderMap)
    {
        item.second->destroy();
    }
    s_shaderMap.clear();

    for (const auto& item : s_modelMap)
    {
        item.second->destroy();
    }
    s_modelMap.clear();
    LOG_INFO("{} assets cleaned up", s_assetCount);
}

void mage::AssetManager::loadMaterials(const char* baseDir)
{
    PROFILER_SCOPE(1);
    LOG_TRACE("Loading materials registered in {}", baseDir);
    int n = 0;
    std::ifstream f(baseDir);
    json j;
    f >> j;

    for (const auto& item : j[ "Materials" ])
    {
        LOG_INFO("Loading material: {}", item[ "id" ].get<std::string>());
        s_materialMap.insert(std::make_pair(item[ "id" ].get<std::string>(),
                                            createScope<Material>(
                                                    item.value("diffuse",
                                                               "./assets/textures/defaults/default_diffuse.png")
                                                        .c_str(),
                                                    item.value("specular",
                                                               "./assets/textures/defaults/default_specular.png")
                                                        .c_str(),
                                                    item.value("emission",
                                                               "./assets/textures/defaults/default_emission.png")
                                                        .c_str(),
                                                    item.value("shininess", 32.0f))));
        n++;
    }
    s_assetCount += n;
    LOG_TRACE("Loaded {} materials", n);
}



