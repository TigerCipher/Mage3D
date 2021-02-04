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

#define RESOLVE_BASE(et) \
	virtual size_t resolve ## et() const NOX;


#define LEAF_ELEMENT(et, systype)                            \
	class et final : public LayoutElement                    \
	{                                                        \
	public:                                                  \
		using SysType = systype;                             \
		size_t resolve ## et() const NOX override final;     \
		size_t getOffsetEnd() const noexcept override final; \
	protected:                                               \
		size_t finish(const size_t offset) override final;   \
		size_t calculateSize() const NOX override final;     \
	};

#define OP_REF(et, ...)                 operator __VA_ARGS__ et::SysType&()NOX;

#define OP_ASSIGN(et)                   et::SysType& operator=(const et::SysType& rhs) NOX;

#define REF_CONST_CONVERSION(et)        OP_REF(et, const)
#define REF_NON_CONST_CONVERSION(et)    OP_REF(et) OP_ASSIGN(et)

#define PTR_CONVERSION(et, ...)         operator __VA_ARGS__ et::SysType*()NOX;

namespace dcb
{
	class LayoutElement
	{
	friend class Struct;
	friend class Array;
	friend class Layout;
	public:
		virtual ~LayoutElement();

		virtual LayoutElement& operator[](const std::string& key);

		virtual const LayoutElement& operator[](const std::string& key) const;


		[[nodiscard]] virtual size_t getOffsetEnd() const noexcept = 0;

		[[nodiscard]] size_t getSizeInBytes() const noexcept;

		[[nodiscard]] size_t getOffsetBegin() const noexcept { return mOffset; }

		virtual LayoutElement& type();

		[[nodiscard]] virtual const LayoutElement& type() const;

		template<typename T>
		LayoutElement& add(const std::string& key) NOX;

		template<typename T>
		LayoutElement& set(const size_t size) NOX;

		static size_t getNextOffset(const size_t offset);

		RESOLVE_BASE(Float4)
		RESOLVE_BASE(Float3)
		RESOLVE_BASE(Float2)
		RESOLVE_BASE(Float)
		RESOLVE_BASE(Bool)
		RESOLVE_BASE(Matrix)

	protected:
		virtual size_t finish(const size_t offset) = 0;
		[[nodiscard]] virtual size_t calculateSize() const NOX = 0;


		size_t mOffset = 0;
	};

	LEAF_ELEMENT(Float4, vec4f)
	LEAF_ELEMENT(Float3, vec3f)
	LEAF_ELEMENT(Float2, vec2f)
	LEAF_ELEMENT(Float, float)
	LEAF_ELEMENT(Matrix, mat4x4)
	LEAF_ELEMENT(Bool, BOOL)



	class Struct : public LayoutElement
	{
	public:
		using LayoutElement::LayoutElement;

		LayoutElement& operator[](const std::string& key) override final;

		[[nodiscard]] size_t getOffsetEnd() const noexcept override final;

		void add(const std::string& name, UniquePtr<LayoutElement> elem) NOX;

	protected:
		size_t finish(const size_t offset) override final;

		[[nodiscard]] size_t calculateSize() const NOX override final;

	private:

		static size_t getPadding(const size_t offset, const size_t size) noexcept;

		std::unordered_map<std::string, LayoutElement*> mMap;
		list<UniquePtr<LayoutElement> > mElements;
	};

	class Array : public LayoutElement
	{
	public:
		using LayoutElement::LayoutElement;

		[[nodiscard]] size_t getOffsetEnd() const noexcept override final;

		void set(UniquePtr<LayoutElement> elem, const size_t size) NOX;

		LayoutElement& type() override final
		{
			return *mElement;
		}

	protected:
		size_t finish(const size_t offset) override final;

		[[nodiscard]] size_t calculateSize() const NOX override final;

	private:
		size_t mSize = 0;
		UniquePtr<LayoutElement> mElement;
	};


	class Layout
	{
	public:
		Layout() : mLayout(createRef<Struct>()) { }

		Layout(SharedPtr<LayoutElement> layout) : mLayout(std::move(layout)) { }

		LayoutElement& operator[](const std::string& key);

		[[nodiscard]] size_t getSizeInBytes() const noexcept;

		template<typename T>
		LayoutElement& add(const std::string& key) NOX
		{
			assert(!mFinished && "Cannot modify a completed layout");
			return mLayout->add<T>(key);
		}

		SharedPtr<LayoutElement> finish();
	private:
		bool mFinished = false;
		SharedPtr<LayoutElement> mLayout;
	};

	class ConstElementRef
	{
	public:
		class ElementPtr
		{
		public:
			ElementPtr(ConstElementRef& ref) : mRef(ref) { }

			PTR_CONVERSION(Float4, const)
			PTR_CONVERSION(Float3, const)
			PTR_CONVERSION(Float2, const)
			PTR_CONVERSION(Float, const)
			PTR_CONVERSION(Bool, const)
			PTR_CONVERSION(Matrix, const)

		private:
			ConstElementRef& mRef;
		};

		ConstElementRef(const LayoutElement* layout, char* bytes, const size_t offset) :
			mLayout(layout),
			mBytes(bytes),
			mOffset(offset) { }

		ConstElementRef operator[](const std::string& key) NOX;

		ConstElementRef operator[](const size_t index) NOX;

		ElementPtr operator&() NOX
		{
			return { *this };
		}

		REF_CONST_CONVERSION(Float4)
		REF_CONST_CONVERSION(Float3)
		REF_CONST_CONVERSION(Float2)
		REF_CONST_CONVERSION(Float)
		REF_CONST_CONVERSION(Matrix)
		REF_CONST_CONVERSION(Bool)

	private:
		const LayoutElement* mLayout;
		char* mBytes;
		size_t mOffset;
	};

	class ElementRef
	{
	public:

		class ElementPtr
		{
		public:
			ElementPtr(ElementRef& ref) : mRef(ref) { }

			PTR_CONVERSION(Float4)
			PTR_CONVERSION(Float3)
			PTR_CONVERSION(Float2)
			PTR_CONVERSION(Float)
			PTR_CONVERSION(Bool)
			PTR_CONVERSION(Matrix)

		private:
			ElementRef& mRef;
		};

		ElementRef(const LayoutElement* layout, char* bytes, const size_t offset) : mLayout(layout),
			mBytes(bytes),
			mOffset(offset) { }

		ElementRef operator[](const std::string& key) const NOX;

		ElementRef operator[](const size_t index) const NOX;

		operator ConstElementRef() const noexcept;

		ElementPtr operator&() NOX
		{
			return { *this };
		}

		REF_NON_CONST_CONVERSION(Float4)
		REF_NON_CONST_CONVERSION(Float3)
		REF_NON_CONST_CONVERSION(Float2)
		REF_NON_CONST_CONVERSION(Float)
		REF_NON_CONST_CONVERSION(Matrix)
		REF_NON_CONST_CONVERSION(Bool)

	private:
		const LayoutElement* mLayout;
		char* mBytes;
		size_t mOffset;
	};

	class Buffer
	{
	public:
		Buffer(Layout& layout) :
			mLayout(std::static_pointer_cast<Struct>(layout.finish())),
			mBytes(mLayout->getOffsetEnd()) { }

		ElementRef operator[](const std::string& key) NOX;

		ConstElementRef operator[](const std::string& key) const NOX;

		[[nodiscard]] const char* getData() const noexcept
		{
			return mBytes.data();
		}

		[[nodiscard]] size_t getSizeInBytes() const noexcept
		{
			return mBytes.size();
		}

		[[nodiscard]] const LayoutElement& getLayout() const noexcept
		{
			return *mLayout;
		}

		[[nodiscard]] SharedPtr<LayoutElement> cloneLayout() const
		{
			return mLayout;
		}

	private:
		SharedPtr<Struct> mLayout;
		list<char> mBytes;
	};

	// LayoutElement::add -> depends on Struct definition, but Struct definition depends on LayoutElement
	template<typename T>
	LayoutElement& LayoutElement::add(const std::string& key) NOX
	{
		auto* s = dynamic_cast<Struct*>(this);
		assert(s != nullptr);
		s->add(key, createScope<T>());
		return *this;
	}


	template <typename T>
	LayoutElement& LayoutElement::set(const size_t size) NOX
	{
		auto* a = dynamic_cast<Array*>(this);
		assert(a != nullptr);
		a->set(createScope<T>(), size);
		return *this;
	}
} // namespace dcb



#undef RESOLVE_BASE
#undef REF_CONST_CONVERSION
#undef REF_NON_CONST_CONVERSION
#undef OP_ASSIGN
#undef OP_REF
#undef PTR_CONVERSION
#undef LEAF_ELEMENT
