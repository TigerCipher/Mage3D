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
 * File Name: DynamicConstantBuffer.cpp
 * Date File Created: 2/4/2021 at 4:00 PM
 * Author: Matt
 */

#include "DynamicConstantBuffer.h"

#define RESOLVE_BASE(et)                            \
	size_t LayoutElement::resolve ## et() const NOX \
	{ assert(false && "Cannot resolve to" #et); return 0; }


#define LEAF_ELEMENT(et, systype)                          \
	size_t et::resolve ## et() const NOX                   \
	{ return getOffsetBegin(); }                           \
	size_t et::getOffsetEnd() const noexcept               \
	{ return getOffsetBegin() + calculateSize(); }         \
	size_t et::finish(const size_t offset)                 \
	{                                                      \
		mOffset = offset; return offset + calculateSize(); \
	}                                                      \
	size_t et::calculateSize() const NOX                   \
	{ return sizeof(SysType); }


#define OP_REF(t, et, ...)                    \
	t::operator __VA_ARGS__ et::SysType&()NOX \
	{ return *reinterpret_cast<et::SysType*>(mBytes + mLayout->resolve ## et()); }

#define OP_ASSIGN(t, et)                                  \
	et::SysType& t::operator=(const et::SysType& rhs) NOX \
	{ return static_cast<et::SysType&>(*this) = rhs; }

#define REF_CONST_CONVERSION(t, et)        OP_REF(t, et, const)
#define REF_NON_CONST_CONVERSION(t, et)    OP_REF(t, et) OP_ASSIGN(t, et)

#define PTR_CONVERSION(t, et, ...)                        \
	t::ElementPtr::operator __VA_ARGS__ et::SysType*()NOX \
	{ return &static_cast<__VA_ARGS__ et::SysType&>(mRef); }


namespace dcb
{
	// LayoutElement class implementations

	LayoutElement::~LayoutElement() = default;

	LayoutElement& LayoutElement::operator[](const std::string& key)
	{
		assert(false && "Cannot access member on non Struct");
		return *this;
	}

	const LayoutElement& LayoutElement::operator[](const std::string& key) const
	{
		return const_cast<LayoutElement&>(*this)[key];
	}

	size_t LayoutElement::getSizeInBytes() const noexcept
	{
		return getOffsetEnd() - getOffsetBegin();
	}

	LayoutElement& LayoutElement::type()
	{
		assert(false);
		return *this;
	}

	const LayoutElement& LayoutElement::type() const
	{
		return const_cast<LayoutElement&>(*this).type();
	}

	size_t LayoutElement::getNextOffset(const size_t offset)
	{
		// HLSL structs are multiples of 16 bytes each
		return offset + (16 - offset % 16) % 16;
	}

	RESOLVE_BASE(Float4)
	RESOLVE_BASE(Float3)
	RESOLVE_BASE(Float2)
	RESOLVE_BASE(Float)
	RESOLVE_BASE(Bool)
	RESOLVE_BASE(Matrix)

	// Type class implementations

	LEAF_ELEMENT(Float4, vec4f)
	LEAF_ELEMENT(Float3, vec3f)
	LEAF_ELEMENT(Float2, vec2f)
	LEAF_ELEMENT(Float, float)
	LEAF_ELEMENT(Matrix, mat4x4)
	LEAF_ELEMENT(Bool, BOOL)


	// Internal fake element class
	class Empty : public LayoutElement
	{
	public:
		size_t getOffsetEnd() const noexcept override
		{
			return 0;
		}

		bool exists() const noexcept override
		{
			return false;
		}

	protected:
		size_t finish(const size_t offset) override
		{
			return 0;
		}

		size_t calculateSize() const NOX override
		{
			return 0;
		}
	private:
		size_t mSize = 0;
		UniquePtr<LayoutElement> mElement;
	} fakeEmptyElement;

	// Struct class implementations

	LayoutElement& Struct::operator[](const std::string& key)
	{
		const auto i = mMap.find(key);
		if(i == mMap.end())
		{
			return fakeEmptyElement;
		}
		return *i->second;
	}

	size_t Struct::getOffsetEnd() const noexcept
	{
		return getNextOffset(mElements.back()->getOffsetEnd());
	}

	void Struct::add(const std::string& name, UniquePtr<LayoutElement> elem) NOX
	{
		mElements.push_back(std::move(elem));
		if (!mMap.emplace(name, mElements.back().get()).second)
		{
			assert(false);
		}
	}

	size_t Struct::finish(const size_t offset)
	{
		assert(!mElements.empty());
		mOffset = offset;
		auto next = offset;
		for (auto& e : mElements)
		{
			next = (*e).finish(next);
		}

		return getOffsetEnd();
	}

	size_t Struct::calculateSize() const NOX
	{
		size_t next = 0;
		for (const auto& e : mElements)
		{
			const auto size = e->calculateSize();
			next += getPadding(next, size) + size;
		}
		return getNextOffset(next);
	}

	size_t Struct::getPadding(const size_t offset, const size_t size) noexcept
	{
		if (offset / 16 != (offset + size - 1) / 16)
		{
			return getNextOffset(offset) - offset;
		}
		return offset;
	}


	// Array class implementations

	size_t Array::getOffsetEnd() const noexcept
	{
		return getOffsetBegin() + getNextOffset(mElement->getSizeInBytes()) * mSize;
	}

	void Array::set(UniquePtr<LayoutElement> elem, const size_t size) NOX
	{
		mElement = std::move(elem);
		mSize = size;
	}

	size_t Array::finish(const size_t offset)
	{
		assert(mSize != 0 && mElement);
		mOffset = offset;
		mElement->finish(offset);
		return offset + mElement->getSizeInBytes() * mSize;
	}

	size_t Array::calculateSize() const NOX
	{
		return getNextOffset(mElement->calculateSize()) * mSize;
	}

	// Layout class implementations

	LayoutElement& Layout::operator[](const std::string& key)
	{
		assert(!mFinished && "Cannot modify a completed layout");
		return (*mLayout)[key];
	}

	size_t Layout::getSizeInBytes() const noexcept
	{
		return mLayout->getSizeInBytes();
	}

	SharedPtr<LayoutElement> Layout::finish()
	{
		mLayout->finish(0);
		mFinished = true;
		return mLayout;
	}

	// ConstElementRef implementations

	PTR_CONVERSION(ConstElementRef, Float4, const)
	PTR_CONVERSION(ConstElementRef, Float3, const)
	PTR_CONVERSION(ConstElementRef, Float2, const)
	PTR_CONVERSION(ConstElementRef, Float, const)
	PTR_CONVERSION(ConstElementRef, Bool, const)
	PTR_CONVERSION(ConstElementRef, Matrix, const)

	ConstElementRef ConstElementRef::operator[](const std::string& key) NOX
	{
		return { &(*mLayout)[key], mBytes, mOffset };
	}

	ConstElementRef ConstElementRef::operator[](const size_t index) NOX
	{
		const auto& t = mLayout->type();
		assert(dynamic_cast<const Array&>(*mLayout).checkIndex(index));
		const auto size = LayoutElement::getNextOffset(t.getSizeInBytes());
		return { &t, mBytes, mOffset + size * index };
	}

	std::optional<ConstElementRef> ConstElementRef::exists() const noexcept
	{
		if (mLayout->exists())
			return ConstElementRef(mLayout, mBytes, mOffset);
		return std::nullopt;
	}

	REF_CONST_CONVERSION(ConstElementRef, Float4)
	REF_CONST_CONVERSION(ConstElementRef, Float3)
	REF_CONST_CONVERSION(ConstElementRef, Float2)
	REF_CONST_CONVERSION(ConstElementRef, Float)
	REF_CONST_CONVERSION(ConstElementRef, Matrix)
	REF_CONST_CONVERSION(ConstElementRef, Bool)

	// ElementRef implementations

	PTR_CONVERSION(ElementRef, Float4)
	PTR_CONVERSION(ElementRef, Float3)
	PTR_CONVERSION(ElementRef, Float2)
	PTR_CONVERSION(ElementRef, Float)
	PTR_CONVERSION(ElementRef, Bool)
	PTR_CONVERSION(ElementRef, Matrix)

	ElementRef ElementRef::operator[](const std::string& key) const NOX
	{
		return { &(*mLayout)[key], mBytes, mOffset };
	}

	ElementRef ElementRef::operator[](const size_t index) const NOX
	{
		const auto& t = mLayout->type();
		assert(static_cast<const Array&>(*mLayout).checkIndex(index));
		const auto size = LayoutElement::getNextOffset(t.getSizeInBytes());
		return { &t, mBytes, mOffset + size * index };
	}

	ElementRef::operator ConstElementRef() const noexcept
	{
		return { mLayout, mBytes, mOffset };
	}

	std::optional<ElementRef> ElementRef::exists() const noexcept
	{
		if (mLayout->exists())
			return ElementRef(mLayout, mBytes, mOffset);
		return std::nullopt;
	}

	REF_NON_CONST_CONVERSION(ElementRef, Float4)
	REF_NON_CONST_CONVERSION(ElementRef, Float3)
	REF_NON_CONST_CONVERSION(ElementRef, Float2)
	REF_NON_CONST_CONVERSION(ElementRef, Float)
	REF_NON_CONST_CONVERSION(ElementRef, Matrix)
	REF_NON_CONST_CONVERSION(ElementRef, Bool)



	// Buffer class implementations


	ElementRef Buffer::operator[](const std::string& key) NOX
	{
		return { &(*mLayout)[key], mBytes.data(), 0 };
	}

	ConstElementRef Buffer::operator[](const std::string& key) const NOX
	{
		return const_cast<Buffer&>(*this)[key];
	}
}
