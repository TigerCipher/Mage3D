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

	enum ElementType
	{
		POSITION2D,
		POSITION3D,
		TEXTURE2D,
		NORMAL,
		COLOR3F,
		COLOR4F,
		COLORARGB
	};

	// Wrapper to retrieve information on the ElementType
	class Element
	{
	public:
		Element(ElementType type, size_t offset) : m_type(type),
			m_offset(offset) { }


		
		//************************************
		// Method:    sizeOf
		// FullName:  mage::Element::sizeOf
		// Access:    public static 
		// Returns:   constexpr size_t
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Parameter: ElementType type
		// Size in bytes of the element type
		//************************************
		static constexpr size_t sizeOf(ElementType type) noexcept(!MAGE_DEBUG)
		{
			switch (type)
			{
			case POSITION2D: return sizeof(vec2f);
			case POSITION3D: return sizeof(vec3f);
			case TEXTURE2D: return sizeof(vec2f);
			case NORMAL: return sizeof(vec3f);
			case COLOR3F: return sizeof(vec3f);
			case COLOR4F: return sizeof(vec4f);
			case COLORARGB: return sizeof(uchar);
			default:
				LOG_ERROR("An invalid element type was used");
				assert("Invalid element type" && false);
				return 0;
			}
		}

		
		//************************************
		// Method:    size
		// FullName:  mage::Element::size
		// Access:    public 
		// Returns:   size_t
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// Size in bytes of the ElementType this Element represents
		//************************************
		size_t size() const noexcept(!MAGE_DEBUG)
		{
			return sizeOf(getType());
		}

		
		//************************************
		// Method:    getOffset
		// FullName:  mage::Element::getOffset
		// Access:    public 
		// Returns:   size_t
		// Qualifier: const
		// The offset where this element begins in the buffer
		//************************************
		size_t getOffset() const { return m_offset; }

		
		//************************************
		// Method:    getOffsetAfter
		// FullName:  mage::Element::getOffsetAfter
		// Access:    public 
		// Returns:   size_t
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// The offset to insert the next element at, also used to retrieve size of buffer
		//************************************
		size_t getOffsetAfter() const noexcept(!MAGE_DEBUG)
		{
			return m_offset + size();
		}


		ElementType getType() const noexcept { return m_type; }

	private:
		ElementType m_type;
		size_t m_offset;
	};


	class VertexLayout
	{
	public:

		//************************************
		// Method:    resolve
		// FullName:  mage::VertexLayout::resolve
		// Access:    public
		// Returns:   const mage::Element&
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// If the supplied type exists in the layout, return it, otherwise
		// if possible return the element at the front of the layout
		//************************************
		template<ElementType type>
		const Element& resolve() const noexcept(!MAGE_DEBUG)
		{
			// If the supplied type exists in the list, return it
			for (auto& e : m_elements)
			{
				if (e.getType() == type)
					return e;
			}
			LOG_ERROR("Failed to resolve element type");
			assert("Could not resolve element type" && false);
			return m_elements.front();
		}


		//************************************
		// Method:    resolve
		// FullName:  mage::VertexLayout::resolve
		// Access:    public
		// Returns:   const mage::Element&
		// Qualifier: const noexcept(!MAGE_DEBUG)
		// Parameter: size_t index
		// Retrieve the element at a specific index
		//************************************
		const Element& resolve(size_t index) const noexcept(!MAGE_DEBUG) { return m_elements[index]; }

		//************************************
		// Method:    append
		// FullName:  mage::VertexLayout::append
		// Access:    public
		// Returns:   mage::VertexLayout&
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Add an element to the layout
		//************************************
		template<ElementType type>
		VertexLayout& append() noexcept(!MAGE_DEBUG)
		{
			m_elements.emplace_back(type, size());
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
			return m_elements.empty() ? 0 : m_elements.back().getOffsetAfter();
		}



	private:
		list<Element> m_elements;
	};


	class Vertex
	{
	friend class VertexBuffer;

	public:

		//************************************
		// Method:    attribute
		// FullName:  mage::Vertex::attribute
		// Access:    public 
		// Returns:   auto&
		// Qualifier: noexcept(!MAGE_DEBUG)
		// Looks for the element so it can be modified
		//************************************
		template<ElementType type>
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

			LOG_ERROR("Bad element type. Could not resolve");
			assert("Bad element type" && false);
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
		// Sets the attribute at a specfic index
		//************************************
		template<typename T>
		void setAttribute(size_t index, T&& value) noexcept(!MAGE_DEBUG)
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
				LOG_ERROR("Bad element type. Could not resolve");
				assert("Bad element type" && false);
			}
		}

	private:


		Vertex(char* data, const VertexLayout& layout) noexcept(!MAGE_DEBUG) :
			m_data(data),
			m_layout(layout)
		{
			assert(data);
		}

		template<typename T, typename ... Args>
		void setAttribute(size_t index, T&& first, Args&&... args) noexcept(!MAGE_DEBUG)
		{
			setAttribute(index, std::forward<T>(first));
			setAttribute(index, std::forward<Args>(args)...);
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


	// TODO #POTENTIAL-BUG: Conflicts with VertexBuffer.h
	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) noexcept(!MAGE_DEBUG) : m_layout(std::move(layout)) { }

		const VertexLayout& getLayout() const noexcept { return m_layout; }

		size_t size() const noexcept(!MAGE_DEBUG) { return m_buffer.size() / m_layout.size(); }

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

		template<typename ... Args>
		void emplaceBack(Args&&... args) noexcept(!MAGE_DEBUG)
		{
			m_buffer.resize(m_buffer.size() + m_layout.size());
			back().setAttribute(0, std::forward<Args>(args)...);
		}

		Vertex operator[](size_t i) noexcept(!MAGE_DEBUG)
		{
			assert(i < size());
			return Vertex{ m_buffer.data() + m_layout.size() * i, m_layout };
		}

	private:
		VertexLayout m_layout;
		list<char> m_buffer;
	};
}