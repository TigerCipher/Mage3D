/*
 * Mage3DX
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
 * File Name: Vertex.h
 * Date File Created: 10/2/2020 at 8:28 PM
 * Author: Matt
 */
#pragma once

#include "MathHelper.h"

#include "Graphics.h"


//TODO: Not everything is template-ized like I originally planned on, can probably move a fair bit to a source file


struct ColorARGB
{
	uchar a;
	uchar r;
	uchar g;
	uchar b;
};

enum AttributeType
{
	POSITION2D,
	POSITION3D,
	TEXTURE2D,
	NORMAL,
	COLOR3F,
	COLOR4F,
	COLORARGB,
	TANGENT,
	BITANGENT
};

// Compile time look up table for the attribute type D3D semantics/formats

template<AttributeType> struct Map;

template<> struct Map<POSITION2D>
{
	static constexpr const char* CODE = "P2";
	static constexpr const char* SEMANTIC = "Position";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32_FLOAT;
	using sysType = vec2f;
};

template<> struct Map<POSITION3D>
{
	static constexpr const char* CODE = "P3";
	static constexpr const char* SEMANTIC = "Position";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
	using sysType = vec3f;
};

template<> struct Map<TEXTURE2D>
{
	static constexpr const char* CODE = "T2";
	static constexpr const char* SEMANTIC = "TexCoords";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32_FLOAT;
	using sysType = vec2f;
};

template<> struct Map<NORMAL>
{
	static constexpr const char* CODE = "N";
	static constexpr const char* SEMANTIC = "Normal";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
	using sysType = vec3f;
};

template<> struct Map<COLOR3F>
{
	static constexpr const char* CODE = "C3";
	static constexpr const char* SEMANTIC = "Color";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
	using sysType = vec3f;
};

template<> struct Map<COLOR4F>
{
	static constexpr const char* CODE = "C4";
	static constexpr const char* SEMANTIC = "Color";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32A32_FLOAT;
	using sysType = vec4f;
};

template<> struct Map<COLORARGB>
{
	static constexpr const char* CODE = "C8";
	static constexpr const char* SEMANTIC = "Color";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
	using sysType = ColorARGB;
};

template<> struct Map<TANGENT>
{
	static constexpr const char* CODE = "Nt";
	static constexpr const char* SEMANTIC = "Tangent";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
	using sysType = vec3f;
};

template<> struct Map<BITANGENT>
{
	static constexpr const char* CODE = "Nb";
	static constexpr const char* SEMANTIC = "Bitangent";
	static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
	using sysType = vec3f;
};



// Wrapper to retrieve information on the AttributeType
class Attribute
{
public:
	Attribute(const AttributeType type, const size_t offset) :
		mType(type),
		mOffset(offset) { }


	/***********************************************************************************
		 *
		 * \brief Retrieves the size, in bytes, of the given attribute type
		 * \param type AttributeType - The attribute type to get the size of
		 * \return size_t The size in bytes
		 *
		 ************************************************************************************/
	static size_t sizeOf(const AttributeType type) noexcept(!MAGE_DEBUG);

	[[nodiscard]] D3D11_INPUT_ELEMENT_DESC getDesc() const noexcept(!MAGE_DEBUG);



	/***********************************************************************************
		 *
		 * \brief The size, in bytes, of this attribute
		 * \return size_t The size in bytes
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t size() const noexcept(!MAGE_DEBUG)
	{
		return sizeOf(getType());
	}




	/***********************************************************************************
		 *
		 * \brief Returns the offset where this attribute begins in the buffer
		 * \return size_t The offset
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t getOffset() const { return mOffset; }


	/***********************************************************************************
		 *
		 * \brief Returns the offset to insert the next attribute at, also used to retrieve size of buffer
		 * \return size_t The offset
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t getOffsetAfter() const noexcept(!MAGE_DEBUG)
	{
		return mOffset + size();
	}


	[[nodiscard]] AttributeType getType() const noexcept { return mType; }

	[[nodiscard]] const char* getCode() const noexcept;
private:

	template<AttributeType Type>
	static constexpr D3D11_INPUT_ELEMENT_DESC getDesc(const size_t offset) noexcept
	{
		return { Map<Type>::SEMANTIC, 0, Map<Type>::FORMAT, 0,
		         static_cast<UINT>(offset),
		         D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}



	AttributeType mType;
	size_t mOffset;
};



class VertexLayout
{
public:

	/***********************************************************************************
		 *
		 * \brief If the supplied type exists in the layout, return it, otherwise
		 * if possible, return the attribute at the front of the layout
		 * \tparam Type The AttributeType
		 * \return const Attribute& The attribute corresponding to the given type
		 *
		 ************************************************************************************/
	template<AttributeType Type>
	const Attribute& resolve() const noexcept(!MAGE_DEBUG)
	{
		// If the supplied type exists in the list, return it
		for (auto& e : mAttribs)
		{
			if (e.getType() == Type)
				return e;
		}
		LOG_ASSERT(false, "Could not resolve attribute type");
		return mAttribs.front();
	}

	/***********************************************************************************
		 *
		 * \brief Returns the attribute at a specific index
		 * \param size_t index The index to retrieve the attribute from
		 * \return const Attribute& The attribute at the specified index
		 *
		 ************************************************************************************/
	[[nodiscard]] const Attribute& resolve(const size_t index) const noexcept(!MAGE_DEBUG) { return mAttribs[index]; }



	//TODO: Change all docs to this format, makes intellisense spit out less warnings. And supports xml (for stuff like line breaks)

	
	/// <summary>
	/// Adds an attribute to the layout
	/// </summary>
	/// <param name="type">The attribute type to add</param>
	/// <returns>A reference to the layout the attribute was added to</returns>
	VertexLayout& append(AttributeType type) noexcept(!MAGE_DEBUG);

	/***********************************************************************************
		 *
		 * \brief Returns the number of vertices in the layout
		 * \return size_t The number of vertices
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t size() const noexcept(!MAGE_DEBUG)
	{
		return mAttribs.empty() ? 0 : mAttribs.back().getOffsetAfter();
	}

	/***********************************************************************************
		 *
		 * \brief Retrieves the number of attributes in this layout
		 * \return size_t The number of attributes in the layout
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t getNumAttributes() const noexcept { return mAttribs.size(); }


	[[nodiscard]] list<D3D11_INPUT_ELEMENT_DESC> getD3dLayout() const noexcept(!MAGE_DEBUG);

	[[nodiscard]] std::string getCode() const noexcept(!MAGE_DEBUG);

private:
	list<Attribute> mAttribs{ };
};


class Vertex
{
friend class VertexBuffer;

public:

	/***********************************************************************************
		 *
		 * \brief Looks for the attribute so it can be modified
		 * \tparam Type The AttributeType
		 * \return auto& A reference to an attribute so it can be modified
		 *
		 ************************************************************************************/
	template<AttributeType Type>
	auto& attribute() const noexcept(!MAGE_DEBUG)
	{
		const auto& elem = mLayout.resolve<Type>();
		auto attrib = mData + elem.getOffset();
		return *reinterpret_cast<typename Map<Type>::sysType*>(attrib);
	}

	/***********************************************************************************
		 *
		 * \brief Sets the attribute at a specific index
		 * \tparam T The value type to set the attribute
		 * \param const size_t index The index of the attribute
		 * \param T&& val The value to set the attribute to
		 *
		 ************************************************************************************/
	template<typename T>
	void setAttributeByIndex(const size_t index, T&& val) noexcept(!MAGE_DEBUG)
	{
		const auto& elem = mLayout.resolve(index);
		auto attrib = mData + elem.getOffset();

		switch (elem.getType())
		{
		case POSITION2D:
			setAttribute<POSITION2D>(attrib, std::forward<T>(val));
			break;
		case POSITION3D:
			setAttribute<POSITION3D>(attrib, std::forward<T>(val));
			break;
		case TEXTURE2D:
			setAttribute<TEXTURE2D>(attrib, std::forward<T>(val));
			break;
		case NORMAL:
			setAttribute<NORMAL>(attrib, std::forward<T>(val));
			break;
		case COLOR3F:
			setAttribute<COLOR3F>(attrib, std::forward<T>(val));
			break;
		case COLOR4F:
			setAttribute<COLOR4F>(attrib, std::forward<T>(val));
			break;
		case COLORARGB:
			setAttribute<COLORARGB>(attrib, std::forward<T>(val));
			break;
		case TANGENT:
			setAttribute<TANGENT>(attrib, std::forward<T>(val));
			break;
		case BITANGENT:
			setAttribute<BITANGENT>(attrib, std::forward<T>(val));
			break;
		default:
			LOG_ASSERT(false, "Bad attribute type");
		}
	}

protected:


	Vertex(char* data, const VertexLayout& layout) noexcept(!MAGE_DEBUG) :
		mData(data),
		mLayout(layout)
	{
		assert(data);
	}

private:
	template<typename T, typename ... Args>
	void setAttributeByIndex(size_t index, T&& first, Args&&... args) noexcept(!MAGE_DEBUG)
	{
		setAttributeByIndex(index, std::forward<T>(first));
		setAttributeByIndex(index + 1, std::forward<Args>(args)...);
	}

	template<AttributeType DestType, typename Src>
	void setAttribute(char* attrib, Src&& val) noexcept(!MAGE_DEBUG)
	{
		using Dest = typename Map<DestType>::sysType;
		if constexpr (std::is_assignable<Dest, Src>::value)
			*reinterpret_cast<Dest*>(attrib) = val;
		else
		{
			LOG_ASSERT(false, "Argument type mismatch");
		}
	}

	const VertexLayout& mLayout;
	char* mData = nullptr;
};


class ConstantVertex
{
public:
	ConstantVertex(const Vertex& v) noexcept(!MAGE_DEBUG) : mVertex(v) { }

	template<AttributeType Type>
	const auto& attribute() const noexcept(!MAGE_DEBUG)
	{
		return const_cast<Vertex&>(mVertex).attribute<Type>();
	}
private:
	Vertex mVertex;
};

class VertexBuffer
{
public:
	VertexBuffer(VertexLayout layout, size_t size = 0) noexcept(!MAGE_DEBUG);

	[[nodiscard]] const VertexLayout& getLayout() const noexcept { return mLayout; }

	void resize(size_t size) noexcept;

	/***********************************************************************************
		 *
		 * \brief Returns the number of vertices in the buffer
		 * \return size_t The number of vertices
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t size() const noexcept(!MAGE_DEBUG) { return mBuffer.size() / mLayout.size(); }


	/***********************************************************************************
		 *
		 * \brief Returns the size of this buffer in bytes
		 * \return size_t The size in bytes
		 *
		 ************************************************************************************/
	[[nodiscard]] size_t sizeInBytes() const noexcept(!MAGE_DEBUG) { return mBuffer.size(); }

	Vertex back() noexcept(!MAGE_DEBUG)
	{
		LOG_ASSERT(!mBuffer.empty(), "Buffer is empty");
		return Vertex{ mBuffer.data() + mBuffer.size() - mLayout.size(), mLayout };
	}

	Vertex front() noexcept(!MAGE_DEBUG)
	{
		LOG_ASSERT(!mBuffer.empty(), "Buffer is empty");
		return Vertex{ mBuffer.data(), mLayout };
	}

	Vertex operator[](const size_t i) noexcept(!MAGE_DEBUG)
	{
		LOG_ASSERT(i < size(), "index out of bounds");
		return Vertex{ mBuffer.data() + mLayout.size() * i, mLayout };
	}

	[[nodiscard]] ConstantVertex back() const noexcept(!MAGE_DEBUG)
	{
		return const_cast<VertexBuffer*>(this)->back();
	}

	[[nodiscard]] ConstantVertex front() const noexcept(!MAGE_DEBUG)
	{
		return const_cast<VertexBuffer*>(this)->front();
	}

	ConstantVertex operator[](const size_t i) const noexcept(!MAGE_DEBUG)
	{
		return const_cast<VertexBuffer&>(*this)[i];
	}

	template<typename ... Args>
	void emplaceBack(Args&&... args) noexcept(!MAGE_DEBUG)
	{
		LOG_ASSERT(sizeof...(args) == mLayout.getNumAttributes(),
			"Argument count does not match expected number of vertex attributes");
		mBuffer.resize(mBuffer.size() + mLayout.size());
		back().setAttributeByIndex(0, std::forward<Args>(args)...);
	}

	[[nodiscard]] const char* getData() const noexcept(!MAGE_DEBUG) { return mBuffer.data(); }

private:
	VertexLayout mLayout;
	list<char> mBuffer;
};
