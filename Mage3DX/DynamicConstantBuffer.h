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

		class Struct& asStruct() NOX;

		RESOLVE_BASE(Float3)
		RESOLVE_BASE(Float)

	private:
		size_t mOffset;
	};

	LEAF_ELEMENT(Float3, vec3f)
	
	LEAF_ELEMENT(Float, float)


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

	class ElementRef
	{
	public:
		ElementRef(const LayoutElement* layout, char* bytes) : mLayout(layout),
			mBytes(bytes) { }
		ElementRef operator[](const std::string& key) const NOX
		{
			return { &(*mLayout)[key], mBytes };
		}

		REF_CONVERSION(Float3, vec3f)
		REF_CONVERSION(Float, float)
	
	private:
		//const class LayoutElement* mLayout;
		const LayoutElement* mLayout;
		char* mBytes;
	};

	class Buffer
	{
	public:
		Buffer(const Struct& layout) : mLayout(&layout),
			mBytes(mLayout->getOffsetEnd()) { }
		
		ElementRef operator[](const std::string& key) NOX
		{
			return { &(*mLayout)[key], mBytes.data() };
		}

	private:
		const Struct* mLayout;
		list<char> mBytes;
	};
} // namespace dcb