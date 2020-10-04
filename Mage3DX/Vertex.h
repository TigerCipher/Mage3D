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



namespace mage
{
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
		COLORARGB
	};

	// Compile time look up table for the attribute type D3D semantics/formats

	template<AttributeType> struct Map;

	template<> struct Map<POSITION2D>
	{
		static constexpr const char* semantic = "Position";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
		using sysType = vec2f;
	};

	template<> struct Map<POSITION3D>
	{
		static constexpr const char* semantic = "Position";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		using sysType = vec3f;
	};

	template<> struct Map<TEXTURE2D>
	{
		static constexpr const char* semantic = "TexCoords";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
		using sysType = vec2f;
	};

	template<> struct Map<NORMAL>
	{
		static constexpr const char* semantic = "Normal";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		using sysType = vec3f;
	};

	template<> struct Map<COLOR3F>
	{
		static constexpr const char* semantic = "Color";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		using sysType = vec3f;
	};

	template<> struct Map<COLOR4F>
	{
		static constexpr const char* semantic = "Color";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		using sysType = vec4f;
	};

	template<> struct Map<COLORARGB>
	{
		static constexpr const char* semantic = "Color";
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
		using sysType = ColorARGB;
	};



	// Wrapper to retrieve information on the AttributeType
	class Attribute
	{
	public:
		Attribute(AttributeType type, size_t offset) : m_type(type),
			m_offset(offset) { }



		//************************************
		// Method:    sizeOf
		// FullName:  mage::Attribute::sizeOf
		// Access:    public static
		// Returns:   constexpr size_t
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Parameter: AttributeType type
		// Size in bytes of the attribute type
		//************************************
		static constexpr size_t sizeOf(AttributeType type) noexcept(!MAGE_DEBUG)
		{
			switch (type)
			{
			case POSITION2D: return sizeof(Map<POSITION2D>::sysType);
			case POSITION3D: return sizeof(Map<POSITION3D>::sysType);
			case TEXTURE2D: return sizeof(Map<TEXTURE2D>::sysType);
			case NORMAL: return sizeof(Map<NORMAL>::sysType);
			case COLOR3F: return sizeof(Map<COLOR3F>::sysType);
			case COLOR4F: return sizeof(Map<COLOR4F>::sysType);
			case COLORARGB: return sizeof(Map<COLORARGB>::sysType);
			default:
				LOG_ERROR("An invalid attribute type was used");
				assert("Invalid attribute type" && false);
				return 0;
			}
		}

		D3D11_INPUT_ELEMENT_DESC getDesc() const noexcept(!MAGE_DEBUG)
		{
			switch (m_type)
			{
			case POSITION2D: return getDesc<POSITION2D>(getOffset());
			case POSITION3D: return getDesc<POSITION3D>(getOffset());
			case TEXTURE2D: return getDesc<TEXTURE2D>(getOffset());
			case NORMAL: return getDesc<NORMAL>(getOffset());
			case COLOR3F: return getDesc<COLOR3F>(getOffset());
			case COLOR4F: return getDesc<COLOR4F>(getOffset());
			case COLORARGB: return getDesc<COLORARGB>(getOffset());
			default:
				LOG_ERROR("Invalid attribute type");
				assert("Invalid attribute type" && false);
				return { "INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		}


		//************************************
		// Method:    size
		// FullName:  mage::Attribute::size
		// Access:    public
		// Returns:   size_t
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// Size in bytes of the AttributeType this Attribute represents
		//************************************
		size_t size() const noexcept(!MAGE_DEBUG)
		{
			return sizeOf(getType());
		}


		//************************************
		// Method:    getOffset
		// FullName:  mage::Attribute::getOffset
		// Access:    public
		// Returns:   size_t
		// Qualifier: const
		// The offset where this attribute begins in the buffer
		//************************************
		size_t getOffset() const { return m_offset; }


		//************************************
		// Method:    getOffsetAfter
		// FullName:  mage::Attribute::getOffsetAfter
		// Access:    public
		// Returns:   size_t
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// The offset to insert the next attribute at, also used to retrieve size of buffer
		//************************************
		size_t getOffsetAfter() const noexcept(!MAGE_DEBUG)
		{
			return m_offset + size();
		}


		AttributeType getType() const noexcept { return m_type; }

	private:

		template<AttributeType type>
		static constexpr D3D11_INPUT_ELEMENT_DESC getDesc(size_t offset) noexcept(!MAGE_DEBUG)
		{
			return { Map<type>::semantic, 0, Map<type>::format, 0, (UINT) offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		AttributeType m_type;
		size_t m_offset;
	};


	class VertexLayout
	{
	public:

		//************************************
		// Method:    resolve
		// FullName:  mage::VertexLayout::resolve
		// Access:    public
		// Returns:   const mage::Attribute&
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// If the supplied type exists in the layout, return it, otherwise
		// if possible return the attribute at the front of the layout
		//************************************
		template<AttributeType type>
		const Attribute& resolve() const noexcept(!MAGE_DEBUG)
		{
			// If the supplied type exists in the list, return it
			for (auto& e : m_attribs)
			{
				if (e.getType() == type)
					return e;
			}
			LOG_ERROR("Failed to resolve attribute type");
			assert("Could not resolve attribute type" && false);
			return m_attribs.front();
		}


		//************************************
		// Method:    resolve
		// FullName:  mage::VertexLayout::resolve
		// Access:    public
		// Returns:   const mage::Attribute&
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// Parameter: size_t index
		// Retrieve the attribute at a specific index
		//************************************
		const Attribute& resolve(size_t index) const noexcept(!MAGE_DEBUG) { return m_attribs[index]; }


		//************************************
		//! \fn append
		//! \brief FullName:  mage::VertexLayout::append
		//! \brief Access:    public
		//!	\brief Qualifier: noexcept(!MAGE_DEBUG)
		//! \return mage::VertexLayout&
		//! \param AttributeType type The attribute type being added to the layout
		//! \brief Add an attribute to the layout
		//************************************
		VertexLayout& append(AttributeType type) noexcept(!MAGE_DEBUG)
		{
			m_attribs.emplace_back(type, size());
			return *this;
		}

		//************************************
		// Method:    size
		// FullName:  mage::VertexLayout::size
		// Access:    public
		// Returns:   size_t
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// Returns the number of vertices in the layout
		//************************************
		size_t size() const noexcept(!MAGE_DEBUG)
		{
			return m_attribs.empty() ? 0 : m_attribs.back().getOffsetAfter();
		}


		//************************************
		// Method:    getNumAttributes
		// FullName:  mage::VertexLayout::getNumAttributes
		// Access:    public
		// Returns:   size_t
		// Qualifier: const noexcept
		// Returns the number of attributes in this layout
		//************************************



		//************************************
		//! \fn getNumAttributes
		//! \brief FullName:  mage::VertexLayout::getNumAttributes
		//! \brief Access:    public
		//!	\brief Qualifier: const noexcept
		//! \return size_t
		//! \brief Returns the number of attributes in this layout
		//************************************
		size_t getNumAttributes() const noexcept { return m_attribs.size(); }


		list<D3D11_INPUT_ELEMENT_DESC> getD3dLayout() const noexcept(!MAGE_DEBUG)
		{
			list< D3D11_INPUT_ELEMENT_DESC> desc;
			for (const auto& a : m_attribs)
			{
				desc.push_back(a.getDesc());
			}
			return desc;
		}



	private:
		list<Attribute> m_attribs;
	};


	class Vertex
	{
	friend class VertexData;

	public:

		//************************************
		// Method:    attribute
		// FullName:  mage::Vertex::attribute
		// Access:    public
		// Returns:   auto&
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Looks for the attribute so it can be modified
		//************************************
		template<AttributeType type>
		auto& attribute() noexcept(!MAGE_DEBUG)
		{
			const auto& elem = m_layout.resolve<type>();
			auto attrib = m_data + elem.getOffset();
			return *reinterpret_cast<typename Map<type>::sysType*>(attrib);
		}

		//************************************
		// Method:    setAttribute
		// FullName:  mage::Vertex::setAttribute
		// Access:    private
		// Returns:   void
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Parameter: size_t index
		// Parameter: T & & value
		// Sets the attribute at a specific index
		//************************************
		template<typename T>
		void setAttributeByIndex(size_t index, T&& val) noexcept(!MAGE_DEBUG)
		{
			const auto& elem = m_layout.resolve(index);
			auto attrib = m_data + elem.getOffset();

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
			default:
				LOG_ERROR("Bad attribute type. Could not resolve");
				assert("Bad attribute type" && false);
			}
		}

	protected:


		Vertex(char* data, const VertexLayout& layout) noexcept(!MAGE_DEBUG) :
			m_data(data),
			m_layout(layout)
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
				LOG_ERROR("Supplied attribute argument type mismatch");
				assert("Argument attribute type mismatch" && false);
			}
		}

		const VertexLayout& m_layout;
		char* m_data = nullptr;
	};


	class ConstantVertex
	{
	public:
		ConstantVertex(const Vertex& v) noexcept(!MAGE_DEBUG) : m_vertex(v) { }

		template<AttributeType type>
		const auto& attribute() const noexcept(!MAGE_DEBUG)
		{
			return const_cast<Vertex&>(m_vertex).attribute<type>();
		}
	private:
		Vertex m_vertex;
	};

	// TODO #POTENTIAL-BUG: Conflicts with VertexBuffer.h
	class VertexData
	{
	public:
		VertexData(VertexLayout layout) noexcept(!MAGE_DEBUG) : m_layout(std::move(layout)) { }

		const VertexLayout& getLayout() const noexcept { return m_layout; }

		//************************************
		//! \fn size
		//! \brief FullName:  mage::VertexData::size
		//! \brief Access:    public
		//!	\brief Qualifier: const noexcept(!MAGE_DEBUG)
		//! \return size_t
		//! \brief Returns the number of vertices in the buffer
		//************************************
		size_t size() const noexcept(!MAGE_DEBUG) { return m_buffer.size() / m_layout.size(); }

		//************************************
		//! \fn sizeInBytes
		//! \brief FullName:  mage::VertexData::sizeInBytes
		//! \brief Access:    public
		//!	\brief Qualifier: const noexcept(!MAGE_DEBUG)
		//! \return size_t
		//! \brief Returns the size of the buffer in bytes
		//************************************
		size_t sizeInBytes() const noexcept(!MAGE_DEBUG) { return m_buffer.size(); }

		Vertex back() noexcept(!MAGE_DEBUG)
		{
			assert(m_buffer.size() != 0);
			return Vertex{ m_buffer.data() + m_buffer.size() - m_layout.size(), m_layout };
		}

		Vertex front() noexcept(!MAGE_DEBUG)
		{
			assert(m_buffer.size() != 0);
			return Vertex{ m_buffer.data(), m_layout };
		}

		Vertex operator[](size_t i) noexcept(!MAGE_DEBUG)
		{
			assert(i < size());
			return Vertex{ m_buffer.data() + m_layout.size() * i, m_layout };
		}

		ConstantVertex back() const noexcept(!MAGE_DEBUG)
		{
			return const_cast<VertexData*>(this)->back();
		}

		ConstantVertex front() const noexcept(!MAGE_DEBUG)
		{
			return const_cast<VertexData*>(this)->front();
		}

		ConstantVertex operator[](size_t i) const noexcept(!MAGE_DEBUG)
		{
			return const_cast<VertexData&>(*this)[i];
		}

		template<typename ... Args>
		void emplaceBack(Args&&... args) noexcept(!MAGE_DEBUG)
		{
			assert(sizeof...(args) == m_layout.getNumAttributes() &&
				"Argument count does not match expected number of vertex attributes");
			m_buffer.resize(m_buffer.size() + m_layout.size());
			back().setAttributeByIndex(0, std::forward<Args>(args)...);
		}

		const char* getData() const noexcept(!MAGE_DEBUG) { return m_buffer.data(); }

	private:
		VertexLayout m_layout;
		list<char> m_buffer;
	};
}