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

namespace dcb
{
	class LayoutElement
	{
	public:
		LayoutElement(const size_t offset) : mOffset(offset) { }

		virtual ~LayoutElement() = default;

		virtual LayoutElement& operator[](const std::string& key)
		{
			// temporary
			assert(false);
			return *this;
		}

		virtual const LayoutElement& operator[](const std::string& key) const
		{
			// temporary
			assert(false);
			return *this;
		}

		[[nodiscard]] size_t getOffsetBegin() const noexcept { return mOffset; }

		[[nodiscard]] virtual size_t getOffsetEnd() const noexcept = 0;

		[[nodiscard]] virtual size_t resolveVec3f() const NOX
		{
			// temporary
			assert(false);
			return 0;
		}

	private:
		size_t mOffset;
	};

	class Vector3 : public LayoutElement
	{
	public:
		[[nodiscard]] size_t resolveVec3f() const NOX override
		{
			return getOffsetBegin();
		}

		[[nodiscard]] size_t getOffsetEnd() const noexcept override
		{
			return getOffsetBegin() + sizeof(vec3f);
		}
	};


	class Struct : public LayoutElement
	{
	public:
		LayoutElement& operator[](const std::string& key) override final
		{
			return *mMap.at(key);
		}

		const LayoutElement& operator[](const std::string& key) const override final
		{
			return *mMap.at(key);
		}

		size_t getOffsetEnd() const noexcept override final
		{
			return mElements.empty() ? getOffsetBegin() : mElements.back()->getOffsetEnd();
		}

		template<typename T>
		void add(const std::string& name)
		{
			mElements.push_back(createScope<T>(getOffsetEnd()));
			if(!mMap.emplace(name, mElements.back().get()).second)
			{
				// temporary
				assert(false);
			}
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

		operator vec3f&() const NOX
		{
			return *reinterpret_cast<vec3f*>(mBytes + mLayout->resolveVec3f());
		}

		vec3f& operator=(const vec3f& rhs) const NOX
		{
			auto& ref = *reinterpret_cast<vec3f*>(mBytes + mLayout->resolveVec3f());
			ref = rhs;
			return ref;
		}
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