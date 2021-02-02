/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: DynamicConstantBuffer.h
 * Date File Created: 2/1/2021 at 10:57 PM
 * Author: Matt
 */

#pragma once

#include "MathHelper.h"

#define RESOLVE_BASE(et)										\
	virtual size_t resolve ## et() const NOX					\
	{	assert(false && "Cannot resolve to" #et); return 0; }


#define LEAF_ELEMENT(et, systype)							\
	class et : public LayoutElement							\
	{														\
	public:													\
		using LayoutElement::LayoutElement;					\
		size_t resolve ## et() const NOX override final		\
		{ return getOffsetBegin(); }						\
		size_t getOffsetEnd() const noexcept override final \
		{ return getOffsetBegin() + sizeof(systype); }		\
	};														\

#define REF_CONVERSION(et, systype)												\
	operator systype&() NOX														\
	{ return *reinterpret_cast<systype*>(mBytes + mLayout->resolve ## et()); }	\
	systype& operator=(const systype& rhs) NOX									\
	{ return static_cast<systype&>(*this) = rhs; }

namespace dcb
{

	class Struct;
	class Array;
	
	class LayoutElement
	{
	public:
		LayoutElement(const size_t offset) : mOffset(offset) { }

		virtual ~LayoutElement() = default;

		virtual LayoutElement& operator[](const std::string& key)
		{
			assert(false && "Cannot access member on non Struct");
			return *this;
		}

		virtual const LayoutElement& operator[](const std::string& key) const
		{
			assert(false && "Cannot access member on non Struct");
			return *this;
		}

		[[nodiscard]] size_t getOffsetBegin() const noexcept { return mOffset; }

		[[nodiscard]] virtual size_t getOffsetEnd() const noexcept = 0;

		[[nodiscard]] size_t getSizeInBytes() const noexcept
		{
			return getOffsetEnd() - getOffsetBegin();
		}

		virtual LayoutElement& type()
		{
			assert(false);
			return *this;
		}

		[[nodiscard]] virtual const LayoutElement& type() const
		{
			assert(false);
			return *this;
		}

		template<typename T>
		Struct& add(const std::string& key) NOX;

		template<typename T>
		Array& set(const size_t size) NOX;

		RESOLVE_BASE(Float4)
		RESOLVE_BASE(Float3)
		RESOLVE_BASE(Float2)
		RESOLVE_BASE(Float)
		RESOLVE_BASE(Bool)
		RESOLVE_BASE(Matrix)

	private:
		size_t mOffset;
	};

	LEAF_ELEMENT(Float4, vec4f)
	LEAF_ELEMENT(Float3, vec3f)
	LEAF_ELEMENT(Float2, vec2f)
	LEAF_ELEMENT(Float, float)
	LEAF_ELEMENT(Matrix, mat4f)
	LEAF_ELEMENT(Bool, BOOL)
	


	class Struct : public LayoutElement
	{
	public:
		using LayoutElement::LayoutElement;
		
		LayoutElement& operator[](const std::string& key) override final
		{
			return *mMap.at(key);
		}

		const LayoutElement& operator[](const std::string& key) const override final
		{
			return *mMap.at(key);
		}

		[[nodiscard]] size_t getOffsetEnd() const noexcept override final
		{
			return mElements.empty() ? getOffsetBegin() : mElements.back()->getOffsetEnd();
		}

		template<typename T>
		Struct& add(const std::string& name) NOX
		{
			mElements.push_back(createScope<T>(getOffsetEnd()));
			if(!mMap.emplace(name, mElements.back().get()).second)
			{
				assert(false);
			}
			return *this;
		}

	private:
		std::unordered_map<std::string, LayoutElement*> mMap;
		list<UniquePtr<LayoutElement> > mElements;
	};

	class Array : public LayoutElement
	{
	public:
		using LayoutElement::LayoutElement;

		[[nodiscard]] size_t getOffsetEnd() const noexcept override final
		{
			assert(mElement);
			return getOffsetBegin() + mElement->getSizeInBytes() * mSize;
		}

		template<typename T>
		Array& set(const size_t size) NOX
		{
			mElement = createScope<T>(getOffsetBegin());
			mSize = size;
			return *this;
		}

		LayoutElement& type() override final
		{
			return *mElement;
		}

		[[nodiscard]] const LayoutElement& type() const override final
		{
			return *mElement;
		}
	
	private:
		size_t mSize = 0;
		UniquePtr<LayoutElement> mElement;
	};

	class ElementRef
	{
	public:
		ElementRef(const LayoutElement* layout, char* bytes, const size_t offset) : mLayout(layout),
			mBytes(bytes), mOffset(offset) { }
		
		ElementRef operator[](const std::string& key) const NOX
		{
			return { &(*mLayout)[key], mBytes, mOffset };
		}

		ElementRef operator[](size_t index) NOX
		{
			const auto& t = mLayout->type();
			return { &t, mBytes, mOffset + t.getSizeInBytes() * index };
		}

		REF_CONVERSION(Float4, vec4f)
		REF_CONVERSION(Float3, vec3f)
		REF_CONVERSION(Float2, vec2f)
		REF_CONVERSION(Float, float)
		REF_CONVERSION(Matrix, mat4f)
		REF_CONVERSION(Bool, BOOL)
	
	private:
		//const class LayoutElement* mLayout;
		const LayoutElement* mLayout;
		char* mBytes;
		size_t mOffset;
	};

	class Buffer
	{
	public:
		Buffer(const Struct& layout) : mLayout(&layout),
			mBytes(mLayout->getOffsetEnd()) { }
		
		ElementRef operator[](const std::string& key) NOX
		{
			return { &(*mLayout)[key], mBytes.data(), 0 };
		}

	private:
		const Struct* mLayout;
		list<char> mBytes;
	};

	// LayoutElement::add -> depends on Struct definition, but Struct definition depends on LayoutElement
	template<typename T>
	Struct& LayoutElement::add(const std::string& key) NOX
	{
		auto* s = dynamic_cast<Struct*>(this);
		assert(s != nullptr);
		return s->add<T>(key);
	}


	template <typename T>
	Array& LayoutElement::set(const size_t size) NOX
	{
		auto* a = dynamic_cast<Array*>(this);
		assert(a != nullptr);
		return a->set<T>(size);
	}
} // namespace dcb