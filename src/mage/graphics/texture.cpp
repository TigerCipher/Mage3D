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
 * File Name: texture.cpp
 * Date File Created: 7/28/2020 at 8:50 PM
 * Author: Matt
 */

#include "mage/graphics/texture.h"
#include <SOIL/SOIL.h>
#include <cassert>
#include <utility>
#include <fmt/format.h>

std::map<std::string, mage::TextureData*> mage::Texture::textureMap;

//mage::Texture::Texture() :
//        m_filePath(TEXTURE_DEFAULT_DIFFUSE),
//        m_type(TEXTURE_DIFFUSE)
//{
//    load(m_filePath.c_str(), GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_NONE);
//}

mage::Texture::Texture(const char* filePath, GLenum textureTarget, GLfloat filter, GLenum internalFormat,
                       GLenum format, bool clamp, GLenum attachment) :
        m_filePath(filePath)
{
    load(filePath, textureTarget, filter, internalFormat, format, clamp, attachment);
}


void mage::Texture::enable(uint slot)
{
    assert(slot < 31 && slot >= 0);
    glActiveTexture(GL_TEXTURE0 + slot);
    m_textureData->bind(0);
}

void mage::Texture::disable(uint slot)
{
    assert(slot < 31 && slot >= 0);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mage::Texture::load(const char* filePath, GLenum textureTarget, GLfloat filter, GLenum internalFormat,
                         GLenum format, bool clamp, GLenum attachment)
{
    std::map<std::string, TextureData*>::const_iterator it = textureMap.find(filePath);
    if (it != textureMap.end())
    {
        m_textureData = it->second;
        m_textureData->addReference();
        //DBGPRINT("Ref count %i", m_textureData->getResourceCount());
    } else
    {
        LOG_TRACE("Loading texture {}", filePath);
        int width, height;
        //TODO Try updating to SOIL2 to see if that better supports DDS files
        ubyte* image = SOIL_load_image(filePath, &width, &height, nullptr, SOIL_LOAD_RGBA);
        if (!image)
        {
            LOG_WARN("Texture ({}) cannot be loaded, reverting to default texture", filePath);
            m_filePath = fmt::format("./assets/textures/defaults/default_{}.png", m_type);
            const char* newPath = m_filePath.c_str();
            image = SOIL_load_image(newPath, &width, &height, nullptr, SOIL_LOAD_RGBA);
        }
        m_textureData = new TextureData(textureTarget, width, height, 1, &image, &filter, &internalFormat,
                                        &format, clamp, &attachment);
        SOIL_free_image_data(image);
        textureMap.insert(std::pair<std::string, TextureData*>(filePath, m_textureData));
    }
}

std::ostream& mage::operator<<(std::ostream& os, const mage::Texture& texture)
{
    os << "m_filePath: " << texture.m_filePath << " m_type: " << texture.m_type << " m_textureData: "
       << *(texture.m_textureData);
    return os;
}

void mage::Texture::enable(uint slot) const
{
    assert(slot < 31 && slot >= 0);
    glActiveTexture(GL_TEXTURE0 + slot);
    m_textureData->bind(0);
}

void mage::Texture::disable(uint slot) const
{
    assert(slot < 31 && slot >= 0);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mage::Texture::destroy()
{
    if (m_filePath.length() > 0)
        textureMap.erase(m_filePath);
    LOG_TRACE("Unloading texture {}", m_filePath);
    while (!m_textureData->removeReference())
    { }
    delete m_textureData;
}

mage::Texture::Texture(const mage::Texture& rhs)
{
    rhs.m_textureData->addReference();
    m_textureData = rhs.m_textureData;
    m_type = rhs.m_type;
    m_filePath = rhs.m_filePath;
}

mage::Texture& mage::Texture::operator=(const mage::Texture& rhs)
{
    rhs.m_textureData->addReference();
    m_textureData = rhs.m_textureData;
    m_type = rhs.m_type;
    m_filePath = rhs.m_filePath;
    return *this;
}

mage::Texture::Texture(int width, int height, ubyte* data, GLenum textureTarget, GLfloat filter,
                       GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
    m_filePath = "";
    m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format,
                                    clamp, &attachment);
}

void mage::Texture::bindAsRenderTarget()
{
    m_textureData->bindAsRenderTarget();
}

void mage::Texture::bindAsRenderTarget() const
{
    m_textureData->bindAsRenderTarget();
}


mage::TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, ubyte** data,
                               GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp,
                               GLenum* attachments)
{
    m_id = new uint[numTextures];
    m_numTextures = numTextures;
    m_textureTarget = textureTarget;
    m_width = width;
    m_height = height;
    m_frameBuffer = 0;
    m_renderBuffer = 0;

    glGenTextures(m_numTextures, m_id);
    for (int i = 0; i < m_numTextures; i++)
    {
        glBindTexture(m_textureTarget, m_id[ i ]);
        glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[ i ]);
        glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[ i ]);

        if (clamp)
        {
            glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glTexImage2D(m_textureTarget, 0, internalFormat[ i ], m_width, m_height, 0, format[ i ],
                     GL_UNSIGNED_BYTE, data[ i ]);
        glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);

    }

    if (attachments != nullptr)
    {
        GLenum drawBuffers[32];
        assert(m_numTextures <= 32);
        bool hasDepth = false;
        for (int i = 0; i < m_numTextures; i++)
        {
            if (attachments[ i ] == GL_DEPTH_ATTACHMENT || attachments[ i ] == GL_STENCIL_ATTACHMENT)
            {
                drawBuffers[ i ] = GL_NONE;
                hasDepth = true;
            } else
                drawBuffers[ i ] = attachments[ i ];
            if (attachments[ i ] == GL_NONE)
                continue;
            if (m_frameBuffer == 0)
            {
                glGenFramebuffers(1, &m_frameBuffer);
                glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[ i ], m_textureTarget, m_id[ i ], 0);
        }

        if (m_frameBuffer != 0)
        {
            //if(!hasDepth)
            //{
            glGenRenderbuffers(1, &m_renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
            //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
            //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                      m_renderBuffer);
            //}

            //glDrawBuffers(m_numTextures, drawBuffers);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_CRITICAL("Error during frame buffer creation");
                assert(false);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
}

mage::TextureData::~TextureData()
{
    LOG_TRACE("Deleting texture id {}", *m_id);
    if (*m_id) glDeleteTextures(m_numTextures, m_id);
    if (m_frameBuffer) glDeleteFramebuffers(1, &m_frameBuffer);
    if (m_renderBuffer) glDeleteRenderbuffers(1, &m_renderBuffer);
    delete[] m_id;
}

void mage::TextureData::bind(int textureNum)
{
    glBindTexture(m_textureTarget, m_id[ textureNum ]);
}

std::ostream& mage::operator<<(std::ostream& os, const mage::TextureData& data)
{
    os << " m_textureTarget: " << data.m_textureTarget
       << " m_id: " << data.m_id[ 0 ] << " m_numTextures: " << data.m_numTextures << " m_width: "
       << data.m_width
       << " m_height: " << data.m_height;
    return os;
}

void mage::TextureData::bindAsRenderTarget() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glViewport(0, 0, m_width, m_height);
}

void mage::TextureCube::load(list<std::string> faces)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    int width, height, channels;
    for (int i = 0; i < faces.size(); i++)
    {

        ubyte* data = SOIL_load_image(faces[ i ].c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
        if(data) LOG_INFO("Cube texture {} loaded", faces[i].c_str());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        SOIL_free_image_data(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

mage::TextureCube::TextureCube(list<std::string> faces)
{
    load(std::move(faces));
}

void mage::TextureCube::enable()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

void mage::TextureCube::disable()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

mage::TextureCube::~TextureCube()
{
    glDeleteTextures(1, &m_id);
}
