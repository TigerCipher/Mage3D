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


		/***********************************************************************************
		 *
		 * \brief Retrieves the size, in bytes, of the given attribute type
		 * \param AttributeType type The attribute type to get the size of
		 * \return size_t The size in bytes
		 *
		 ************************************************************************************/
		static constexpr size_t sizeOf(const AttributeType type) noexcept(!MAGE_DEBUG)
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

		[[nodiscard]] D3D11_INPUT_ELEMENT_DESC getDesc() const noexcept(!MAGE_DEBUG)
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
				return { "INVALID", 0, DXGI_FORMAT_UNKNOWN,
				         0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		}



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
		size_t getOffset() const { return m_offset; }


		/***********************************************************************************
		 *
		 * \brief Returns the offset to insert the next attribute at, also used to retrieve size of buffer
		 * \return size_t The offset
		 *
		 ************************************************************************************/
		[[nodiscard]] size_t getOffsetAfter() const noexcept(!MAGE_DEBUG)
		{
			return m_offset + size();
		}


		[[nodiscard]] AttributeType getType() const noexcept { return m_type; }

	private:

		template<AttributeType Type>
		static constexpr D3D11_INPUT_ELEMENT_DESC getDesc(const size_t offset) noexcept(!MAGE_DEBUG)
		{
			return { Map<Type>::semantic, 0, Map<Type>::format, 0, (UINT) offset,
			         D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		AttributeType m_type;
		size_t m_offset;
	};


	class VertexLayout
	{
	public:

		/***********************************************************************************
		 *
		 * \brief If the supplied type exists in the layout, return it, otherwise
		 * if possible, return the attribute at the front of the layout
		 * \tparam Type The AttributeType
		 * \return const mage::Attribute& The attribute corresponding to the given type
		 *
		 ************************************************************************************/
		template<AttributeType Type>
		const Attribute& resolve() const noexcept(!MAGE_DEBUG)
		{
			// If the supplied type exists in the list, return it
			for (auto& e : m_attribs)
			{
				if (e.getType() == Type)
					return e;
			}
			LOG_ERROR("Failed to resolve attribute type");
			assert("Could not resolve attribute type" && false);
			return m_attribs.front();
		}

		/***********************************************************************************
		 *
		 * \brief Returns the attribute at a specific index
		 * \param size_t index The index to retrieve the attribute from
		 * \return const mage::Attribute& The attribute at the specified index
		 *
		 ************************************************************************************/
		const Attribute& resolve(size_t index) const noexcept(!MAGE_DEBUG) { return m_attribs[index]; }


		/***********************************************************************************
		 *
		 * \brief Adds an attribute to the layout
		 * \param mage::AttributeType type The attribute type being added to the layout
		 * \return mage::VertexLayout& A reference to the layout the attribute was added to
		 *
		 ************************************************************************************/
		VertexLayout& append(AttributeType type) noexcept(!MAGE_DEBUG)
		{
			m_attribs.emplace_back(type, size());
			return *this;
		}

		/***********************************************************************************
		 *
		 * \brief Returns the number of vertices in the layout
		 * \return size_t The number of vertices
		 *
		 ************************************************************************************/
		[[nodiscard]] size_t size() const noexcept(!MAGE_DEBUG)
		{
			return m_attribs.empty() ? 0 : m_attribs.back().getOffsetAfter();
		}

		/***********************************************************************************
		 *
		 * \brief Retrieves the number of attributes in this layout
		 * \return size_t The number of attributes in the layout
		 *
		 ************************************************************************************/
		[[nodiscard]] size_t getNumAttributes() const noexcept { return m_attribs.size(); }


		[[nodiscard]] list<D3D11_INPUT_ELEMENT_DESC> getD3dLayout() const noexcept(!MAGE_DEBUG)
		{
			list< D3D11_INPUT_ELEMENT_DESC> desc;
			for (const auto& a : m_attribs)
			{
				desc.push_back(a.getDesc());
			}
			return desc;
		}



	private:
		list<Attribute> m_attribs{ };
	};


	class Vertex
	{
	friend class VertexData;

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
			const auto& elem = m_layout.resolve<Type>();
			auto attrib = m_data + elem.getOffset();
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

	class VertexData
	{
	public:
		VertexData(VertexLayout layout) noexcept(!MAGE_DEBUG) : m_layout(std::move(layout)) { }

		[[nodiscard]] const VertexLayout& getLayout() const noexcept { return m_layout; }

		
		/***********************************************************************************
		 * 
		 * \brief Returns the number of vertices in the buffer
		 * \return size_t The number of vertices
		 *
		 ************************************************************************************/
		[[nodiscard]] size_t size() const noexcept(!MAGE_DEBUG) { return m_buffer.size() / m_layout.size(); }


		/***********************************************************************************
		 * 
		 * \brief Returns the size of this buffer in bytes
		 * \return size_t The size in bytes
		 *
		 ************************************************************************************/
		[[nodiscard]] size_t sizeInBytes() const noexcept(!MAGE_DEBUG) { return m_buffer.size(); }

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

		Vertex operator[](const size_t i) noexcept(!MAGE_DEBUG)
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

		ConstantVertex operator[](const size_t i) const noexcept(!MAGE_DEBUG)
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