#pragma once

#include "MathHelper.h"
#include "Log.h"
#include <bmd/types.h>


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
			case POSITION2D: return sizeof(vec2f);
			case POSITION3D: return sizeof(vec3f);
			case TEXTURE2D: return sizeof(vec2f);
			case NORMAL: return sizeof(vec3f);
			case COLOR3F: return sizeof(vec3f);
			case COLOR4F: return sizeof(vec4f);
			case COLORARGB: return sizeof(ColorARGB);
			default:
				LOG_ERROR("An invalid attribute type was used");
				assert("Invalid attribute type" && false);
				return 0;
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
		// Method:    append
		// FullName:  mage::VertexLayout::append
		// Access:    public
		// Returns:   mage::VertexLayout&
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Add an attribute to the layout
		//************************************
		template<AttributeType type>
		VertexLayout& append() noexcept(!MAGE_DEBUG)
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

			if constexpr (type == POSITION2D)
				return *reinterpret_cast<vec2f*>(attrib);
			else if constexpr (type == POSITION3D)
				return *reinterpret_cast<vec3f*>(attrib);
			else if constexpr (type == TEXTURE2D)
				return *reinterpret_cast<vec2f*>(attrib);
			else if constexpr (type == NORMAL)
				return *reinterpret_cast<vec3f*>(attrib);
			else if constexpr (type == COLOR3F)
				return *reinterpret_cast<vec3f*>(attrib);
			else if constexpr (type == COLOR4F)
				return *reinterpret_cast<vec4f*>(attrib);
			else if constexpr (type == COLORARGB)
				return *reinterpret_cast<ColorARGB*>(attrib);

			LOG_ERROR("Bad attribute type. Could not resolve");
			assert("Bad attribute type" && false);
			return *reinterpret_cast<char*>(attrib);
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
		void setAttribute(size_t index, T&& val) noexcept(!MAGE_DEBUG)
		{
			const auto& elem = m_layout.resolve(index);
			auto attrib = m_data + elem.getOffset();

			switch (elem.getType())
			{
			case POSITION2D:
				setAttribute<vec2f>(attrib, std::forward<T>(val));
				break;
			case POSITION3D:
				setAttribute<vec3f>(attrib, std::forward<T>(val));
				break;
			case TEXTURE2D:
				setAttribute<vec2f>(attrib, std::forward<T>(val));
				break;
			case NORMAL:
				setAttribute<vec3f>(attrib, std::forward<T>(val));
				break;
			case COLOR3F:
				setAttribute<vec3f>(attrib, std::forward<T>(val));
				break;
			case COLOR4F:
				setAttribute<vec4f>(attrib, std::forward<T>(val));
				break;
			case COLORARGB:
				setAttribute<ColorARGB>(attrib, std::forward<T>(val));
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
		void setAttribute(size_t index, T&& first, Args&&... args) noexcept(!MAGE_DEBUG)
		{
			setAttribute(index, std::forward<T>(first));
			setAttribute(index + 1, std::forward<Args>(args)...);
		}

		template<typename Dest, typename Src>
		void setAttribute(char* attrib, Src&& val) noexcept(!MAGE_DEBUG)
		{
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
		ConstantVertex(const Vertex& v) noexcept(!MAGE_DEBUG) : m_vertex(v) {}

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

		size_t size() const noexcept(!MAGE_DEBUG) { return m_buffer.size() / m_layout.size(); }

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
			back().setAttribute(0, std::forward<Args>(args)...);
		}

		const char* getData() const noexcept(!MAGE_DEBUG) { return m_buffer.data(); }

	private:
		VertexLayout m_layout;
		list<char> m_buffer;
	};
}