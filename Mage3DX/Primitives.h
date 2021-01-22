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
 * File Name: Primitives.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include "MathHelper.h"
#include "Vertex.h"

#ifdef MAGE_DEBUG
	#define SOMETHING
	#ifdef MAGE_VERBOSE
		#define SOMETHING_ELSE
	#endif
#endif

class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;

	IndexedTriangleList(VertexBuffer verts, list<ushort> ints) :
		vertices(std::move(verts)),
		indices(std::move(ints))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void transform(mat4f matrix)
	{
		
		for (int i = 0; i < vertices.size(); i++)
		{
			auto& pos = vertices[i].attribute<AttributeType::POSITION3D>();
			dx::XMStoreFloat3(&pos, dx::XMVector3Transform(dx::XMLoadFloat3(&pos), matrix));
		}
	}

	//void setNormalsFlat() noexcept(!MAGE_DEBUG)
	//{
	//	assert(indices.size() % 3 == 0 && indices.size() > 0);
	//	for (size_t i = 0; i < indices.size(); i += 3)
	//	{
	//		auto& v0 = vertices[ indices[ i ] ];
	//		auto& v1 = vertices[ indices[ i + 1 ] ];
	//		auto& v2 = vertices[ indices[ i + 2 ] ];
	//		const auto p0 = dx::XMLoadFloat3(&v0.pos);
	//		const auto p1 = dx::XMLoadFloat3(&v1.pos);
	//		const auto p2 = dx::XMLoadFloat3(&v2.pos);

	//		const auto n = dx::XMVector3Normalize(
	//			dx::XMVector3Cross(dx::XMVectorSubtract(p1, p0),
	//				dx::XMVectorSubtract(p2, p0)));

	//		dx::XMStoreFloat3(&v0.normal, n);
	//		dx::XMStoreFloat3(&v1.normal, n);
	//		dx::XMStoreFloat3(&v2.normal, n);
	//	}
	//}

	VertexBuffer vertices;
	list<ushort> indices;
};

//class Cone
//{
//public:
//	template<class V>
//	static IndexedTriangleList<V> makeTesselated(int longDiv)
//	{
//		assert(longDiv >= 3);
//
//		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
//		const float longitudeAngle = 2.0f * PI / (float) longDiv;
//
//		// base vertices
//		list<V> vertices;
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			vertices.emplace_back();
//			auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
//			dx::XMStoreFloat3(&vertices.back().pos, v);
//		}
//		// the center
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
//		const auto iCenter = (ushort) (vertices.size() - 1);
//		// the tip :darkness:
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
//		const auto iTip = (ushort) (vertices.size() - 1);
//
//
//		// base indices
//		std::vector<ushort> indices;
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			indices.push_back(iCenter);
//			indices.push_back((iLong + 1) % longDiv);
//			indices.push_back(iLong);
//		}
//
//		// cone indices
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			indices.push_back(iLong);
//			indices.push_back((iLong + 1) % longDiv);
//			indices.push_back(iTip);
//		}
//
//		return { std::move(vertices), std::move(indices) };
//	}
//
//	template <class V>
//	static IndexedTriangleList<V> makeTesselatedIndependentFaces(int longDiv)
//	{
//		assert(longDiv >= 3);
//
//		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
//		const float longitudeAngle = 2.0f * PI / longDiv;
//
//		list<V> vertices;
//
//		// cone vertices
//		const auto iCone = (ushort) vertices.size();
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			const float thetas[] = {
//				longitudeAngle* iLong,
//				longitudeAngle* (((iLong + 1) == longDiv) ? 0 : (iLong + 1))
//			};
//			vertices.emplace_back();
//			vertices.back().pos = { 0.0f,0.0f,1.0f };
//			for (auto theta : thetas)
//			{
//				vertices.emplace_back();
//				const auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(theta));
//				dx::XMStoreFloat3(&vertices.back().pos, v);
//			}
//		}
//		// base vertices
//		const auto iBaseCenter = (ushort) vertices.size();
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f,0.0f,-1.0f };
//		const auto iBaseEdge = (ushort) vertices.size();
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			vertices.emplace_back();
//			auto v = dx::XMVector3Transform(
//				base,
//				dx::XMMatrixRotationZ(longitudeAngle * iLong)
//				);
//			dx::XMStoreFloat3(&vertices.back().pos, v);
//		}
//
//		list<ushort> indices;
//
//		// cone indices
//		for (ushort i = 0; i < longDiv * 3; i++)
//		{
//			indices.push_back(i + iCone);
//		}
//		// base indices
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			indices.push_back(iBaseCenter);
//			indices.push_back((iLong + 1) % longDiv + iBaseEdge);
//			indices.push_back(iLong + iBaseEdge);
//		}
//
//		return { std::move(vertices),std::move(indices) };
//	}
//
//	template<class V>
//	static IndexedTriangleList<V> make()
//	{
//		return makeTesselated<V>(24);
//	}
//};
//
//
//class Cube
//{
//public:
//	template<class V>
//	static IndexedTriangleList<V> make()
//	{
//		constexpr float side = 1.0f / 2.0f;
//
//		list<vec3f> vertices;
//		vertices.emplace_back(-side, -side, -side);
//		vertices.emplace_back(side,  -side, -side);
//		vertices.emplace_back(-side, side,  -side);
//		vertices.emplace_back(side,  side,  -side);
//		vertices.emplace_back(-side, -side, side);
//		vertices.emplace_back(side,  -side, side);
//		vertices.emplace_back(-side, side,  side);
//		vertices.emplace_back(side,  side,  side);
//
//		list<V> verts(vertices.size());
//		for (size_t i = 0; i < vertices.size(); i++)
//		{
//			verts[ i ].pos = vertices[ i ];
//		}
//		return {
//		    std::move(verts),
//			{
//				0, 2, 1,
//				2, 3, 1,
//				1, 3, 5,
//				3, 7, 5,
//				2, 6, 3,
//				3, 6, 7,
//				4, 5, 7,
//				4, 7, 6,
//				0, 4, 2,
//				2, 4, 6,
//				0, 1, 4,
//				1, 5, 4,
//			}
//		};
//	}
//
//	template<class V>
//	static IndexedTriangleList<V> makeSkinned()
//	{
//		constexpr float side = 1.0f / 2.0f;
//		list<V> vertices(14);
//
//		vertices[ 0 ].pos = { -side, -side, -side };
//		vertices[ 0 ].tex = { 2.0f / 3.0f, 0.0f / 4.0f };
//		vertices[ 1 ].pos = { side, -side, -side };
//		vertices[ 1 ].tex = { 1.0f / 3.0f, 0.0f / 4.0f };
//		vertices[ 2 ].pos = { -side, side, -side };
//		vertices[ 2 ].tex = { 2.0f / 3.0f, 1.0f / 4.0f };
//		vertices[ 3 ].pos = { side, side, -side };
//		vertices[ 3 ].tex = { 1.0f / 3.0f, 1.0f / 4.0f };
//		vertices[ 4 ].pos = { -side, -side, side };
//		vertices[ 4 ].tex = { 2.0f / 3.0f, 3.0f / 4.0f };
//		vertices[ 5 ].pos = { side, -side, side };
//		vertices[ 5 ].tex = { 1.0f / 3.0f, 3.0f / 4.0f };
//		vertices[ 6 ].pos = { -side, side, side };
//		vertices[ 6 ].tex = { 2.0f / 3.0f, 2.0f / 4.0f };
//		vertices[ 7 ].pos = { side, side, side };
//		vertices[ 7 ].tex = { 1.0f / 3.0f, 2.0f / 4.0f };
//		vertices[ 8 ].pos = { -side, -side, -side };
//		vertices[ 8 ].tex = { 2.0f / 3.0f, 4.0f / 4.0f };
//		vertices[ 9 ].pos = { side, -side, -side };
//		vertices[ 9 ].tex = { 1.0f / 3.0f, 4.0f / 4.0f };
//		vertices[ 10 ].pos = { -side, -side, -side };
//		vertices[ 10 ].tex = { 3.0f / 3.0f, 1.0f / 4.0f };
//		vertices[ 11 ].pos = { -side, -side, side };
//		vertices[ 11 ].tex = { 3.0f / 3.0f, 2.0f / 4.0f };
//		vertices[ 12 ].pos = { side, -side, -side };
//		vertices[ 12 ].tex = { 0.0f / 3.0f, 1.0f / 4.0f };
//		vertices[ 13 ].pos = { side, -side, side };
//		vertices[ 13 ].tex = { 0.0f / 3.0f, 2.0f / 4.0f };
//
//		return {
//		    std::move(vertices),
//			{
//				0, 2, 1,    2, 3, 1,
//				4, 8, 5,    5, 8, 9,
//				2, 6, 3,    3, 6, 7,
//				4, 5, 7,    4, 7, 6,
//				2, 10, 11,  2, 11, 6,
//				12, 3, 7,   12, 7, 13
//			}
//		};
//	}
//
//	template<class V>
//	static IndexedTriangleList<V> makeIndependent()
//	{
//		constexpr float side = 1.0f / 2.0f;
//
//		std::vector<V> vertices(24);
//
//		// Near
//		vertices[ 0 ].pos = { -side,-side,-side };
//		vertices[ 1 ].pos = { side,-side,-side };
//		vertices[ 2 ].pos = { -side,side,-side };
//		vertices[ 3 ].pos = { side,side,-side };
//
//		// Far
//		vertices[ 4 ].pos = { -side,-side,side };
//		vertices[ 5 ].pos = { side,-side,side };
//		vertices[ 6 ].pos = { -side,side,side };
//		vertices[ 7 ].pos = { side,side,side };
//
//		// Left
//		vertices[ 8 ].pos = { -side,-side,-side };
//		vertices[ 9 ].pos = { -side,side,-side };
//		vertices[ 10 ].pos = { -side,-side,side };
//		vertices[ 11 ].pos = { -side,side,side };
//
//		// Right
//		vertices[ 12 ].pos = { side,-side,-side };
//		vertices[ 13 ].pos = { side,side,-side };
//		vertices[ 14 ].pos = { side,-side,side };
//		vertices[ 15 ].pos = { side,side,side };
//
//		// Bottom
//		vertices[ 16 ].pos = { -side,-side,-side };
//		vertices[ 17 ].pos = { side,-side,-side };
//		vertices[ 18 ].pos = { -side,-side,side };
//		vertices[ 19 ].pos = { side,-side,side };
//
//		// Top
//		vertices[ 20 ].pos = { -side,side,-side };
//		vertices[ 21 ].pos = { side,side,-side };
//		vertices[ 22 ].pos = { -side,side,side };
//		vertices[ 23 ].pos = { side,side,side };
//
//		return{
//		    std::move(vertices),
//			{
//				0, 2, 1,        2, 3, 1,
//				4, 5, 7,        4, 7, 6,
//				8, 10, 9,       10, 11, 9,
//				12, 13, 15,     12, 15, 14,
//				16, 17, 18,     18, 17, 19,
//				20, 23, 21,     20, 22, 23
//			}
//		};
//	}
//
//	template <class V>
//	static IndexedTriangleList<V> makeIndependentTextured()
//	{
//		auto initial = makeIndependent<V>();
//
//		initial.vertices[0].texCoords = { 0.0f,0.0f };
//		initial.vertices[1].texCoords = { 1.0f,0.0f };
//		initial.vertices[2].texCoords = { 0.0f,1.0f };
//		initial.vertices[3].texCoords = { 1.0f,1.0f };
//		initial.vertices[4].texCoords = { 0.0f,0.0f };
//		initial.vertices[5].texCoords = { 1.0f,0.0f };
//		initial.vertices[6].texCoords = { 0.0f,1.0f };
//		initial.vertices[7].texCoords = { 1.0f,1.0f };
//		initial.vertices[8].texCoords = { 0.0f,0.0f };
//		initial.vertices[9].texCoords = { 1.0f,0.0f };
//		initial.vertices[10].texCoords = { 0.0f,1.0f };
//		initial.vertices[11].texCoords = { 1.0f,1.0f };
//		initial.vertices[12].texCoords = { 0.0f,0.0f };
//		initial.vertices[13].texCoords = { 1.0f,0.0f };
//		initial.vertices[14].texCoords = { 0.0f,1.0f };
//		initial.vertices[15].texCoords = { 1.0f,1.0f };
//		initial.vertices[16].texCoords = { 0.0f,0.0f };
//		initial.vertices[17].texCoords = { 1.0f,0.0f };
//		initial.vertices[18].texCoords = { 0.0f,1.0f };
//		initial.vertices[19].texCoords = { 1.0f,1.0f };
//		initial.vertices[20].texCoords = { 0.0f,0.0f };
//		initial.vertices[21].texCoords = { 1.0f,0.0f };
//		initial.vertices[22].texCoords = { 0.0f,1.0f };
//		initial.vertices[23].texCoords = { 1.0f,1.0f };
//
//		return initial;
//	}
//};
//
//class Plane
//{
//public:
//	template<class V>
//	static IndexedTriangleList<V> makeTesselated(int divisionsX, int divisionsY)
//	{
//		assert(divisionsX >= 1);
//		assert(divisionsY >= 1);
//
//		constexpr float width = 2.0f;
//		constexpr float height = 2.0f;
//		const int numVertsX = divisionsX + 1;
//		const int numVertsY = divisionsY + 1;
//		list<V> vertices(numVertsX * numVertsY);
//
//		{
//			const float sideX = width / 2.0f;
//			const float sideY = height / 2.0f;
//			const float divisionSizeX = width / float(divisionsX);
//			const float divisionSizeY = height / float(divisionsY);
//			const auto bottomLeft = dx::XMVectorSet(-sideX, -sideY, 0.0f, 0.0f);
//
//			for (int y = 0, i = 0; y < numVertsY; y++)
//			{
//				const float yPos = float(y) * divisionSizeY;
//				for (int x = 0; x < numVertsX; x++, i++)
//				{
//					const auto v = dx::XMVectorAdd(bottomLeft,
//						dx::XMVectorSet(float(x) * divisionSizeX, yPos, 0.0f, 0.0f));
//					dx::XMStoreFloat3(&vertices[ i ].pos, v);
//				}
//			}
//		}
//
//		list<ushort> indices;
//		indices.reserve(pow2(divisionsX * divisionsY) * 6);
//		{
//			const auto vxy2i = [ numVertsX ] (size_t x, size_t y) {
//					return (ushort) (y * numVertsX + x);
//				};
//			for (size_t y = 0; y < divisionsY; y++)
//			{
//				for (size_t x = 0; x < divisionsX; x++)
//				{
//					const std::array<ushort, 4> indexArray = {
//						vxy2i(x,     y), vxy2i(x + 1, y), vxy2i(x, y + 1),
//						vxy2i(x + 1, y + 1)
//					};
//					indices.push_back(indexArray[ 0 ]);
//					indices.push_back(indexArray[ 2 ]);
//					indices.push_back(indexArray[ 1 ]);
//					indices.push_back(indexArray[ 1 ]);
//					indices.push_back(indexArray[ 2 ]);
//					indices.push_back(indexArray[ 3 ]);
//				}
//			}
//		}
//
//		return { std::move(vertices), std::move(indices) };
//	}
//
//	template<class V>
//	static IndexedTriangleList<V> make()
//	{
//		return makeTesselated<V>(1, 1);
//	}
//};
//
//class Prism
//{
//public:
//	template<class V>
//	static IndexedTriangleList<V> makeTesselated(int longDiv)
//	{
//		assert(longDiv >= 3);
//
//		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
//		const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
//		const float longitudeAngle = 2.0f * PI / (float) longDiv;
//
//		// near center
//		list<V> vertices;
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
//		const auto iCenterNear = (ushort) (vertices.size() - 1);
//		// far center
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
//		const auto iCenterFar = (ushort) (vertices.size() - 1);
//
//		// base vertices
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			// near base
//			{
//				vertices.emplace_back();
//				auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
//				dx::XMStoreFloat3(&vertices.back().pos, v);
//			}
//			// far base
//			{
//				vertices.emplace_back();
//				auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
//				v = dx::XMVectorAdd(v, offset);
//				dx::XMStoreFloat3(&vertices.back().pos, v);
//			}
//		}
//
//		// side indices
//		list<ushort> indices;
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			const auto i = iLong * 2;
//			const auto mod = longDiv * 2;
//			indices.push_back(i + 2);
//			indices.push_back((i + 2) % mod + 2);
//			indices.push_back(i + 1 + 2);
//			indices.push_back((i + 2) % mod + 2);
//			indices.push_back((i + 3) % mod + 2);
//			indices.push_back(i + 1 + 2);
//		}
//
//		// base indices
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			const auto i = iLong * 2;
//			const auto mod = longDiv * 2;
//			indices.push_back(i + 2);
//			indices.push_back(iCenterNear);
//			indices.push_back((i + 2) % mod + 2);
//			indices.push_back(iCenterFar);
//			indices.push_back(i + 1 + 2);
//			indices.push_back((i + 3) % mod + 2);
//		}
//
//		return { std::move(vertices), std::move(indices) };
//	}
//
//	template<class V>
//	static IndexedTriangleList<V> makeTesselatedNormalsNoCaps(int longDiv)
//	{
//		assert(longDiv >= 3);
//
//		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
//		const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
//		const float longitudeAngle = 2.0f * PI / longDiv;
//
//		list<V> vertices;
//
//		// near center
//		const auto iCenterNear = (ushort) vertices.size();
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f,0.0f,-1.0f };
//		vertices.back().n = { 0.0f,0.0f,-1.0f };
//		// near base vertices
//		const auto iBaseNear = (ushort) vertices.size();
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			vertices.emplace_back();
//			auto v = dx::XMVector3Transform(
//				base,
//				dx::XMMatrixRotationZ(longitudeAngle * iLong)
//				);
//			dx::XMStoreFloat3(&vertices.back().pos, v);
//			vertices.back().n = { 0.0f,0.0f,-1.0f };
//		}
//		// far center
//		const auto iCenterFar = (ushort) vertices.size();
//		vertices.emplace_back();
//		vertices.back().pos = { 0.0f,0.0f,1.0f };
//		vertices.back().n = { 0.0f,0.0f,1.0f };
//		// far base vertices
//		const auto iBaseFar = (ushort) vertices.size();
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			vertices.emplace_back();
//			auto v = dx::XMVector3Transform(
//				base,
//				dx::XMMatrixRotationZ(longitudeAngle * iLong)
//				);
//			v = dx::XMVectorAdd(v, offset);
//			dx::XMStoreFloat3(&vertices.back().pos, v);
//			vertices.back().n = { 0.0f,0.0f,1.0f };
//		}
//		// fuselage vertices
//		const auto iFuselage = (ushort) vertices.size();
//		for (int iLong = 0; iLong < longDiv; iLong++)
//		{
//			// near base
//			{
//				vertices.emplace_back();
//				auto v = dx::XMVector3Transform(
//					base,
//					dx::XMMatrixRotationZ(longitudeAngle * iLong)
//					);
//				dx::XMStoreFloat3(&vertices.back().pos, v);
//				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
//			}
//			// far base
//			{
//				vertices.emplace_back();
//				auto v = dx::XMVector3Transform(base,
//					dx::XMMatrixRotationZ(longitudeAngle * iLong));
//
//				v = dx::XMVectorAdd(v, offset);
//				dx::XMStoreFloat3(&vertices.back().pos, v);
//				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
//			}
//		}
//
//		list<ushort> indices;
//
//		// near base indices
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			const auto i = iLong;
//			const auto mod = longDiv;
//			// near
//			indices.push_back(i + iBaseNear);
//			indices.push_back(iCenterNear);
//			indices.push_back((i + 1) % mod + iBaseNear);
//		}
//		// far base indices
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			const auto i = iLong;
//			const auto mod = longDiv;
//			// far
//			indices.push_back(iCenterFar);
//			indices.push_back(i + iBaseFar);
//			indices.push_back((i + 1) % mod + iBaseFar);
//		}
//		// fuselage indices
//		for (ushort iLong = 0; iLong < longDiv; iLong++)
//		{
//			const auto i = iLong * 2;
//			const auto mod = longDiv * 2;
//			indices.push_back(i + iFuselage);
//			indices.push_back((i + 2) % mod + iFuselage);
//			indices.push_back(i + 1 + iFuselage);
//			indices.push_back((i + 2) % mod + iFuselage);
//			indices.push_back((i + 3) % mod + iFuselage);
//			indices.push_back(i + 1 + iFuselage);
//		}
//
//		return { std::move(vertices),std::move(indices) };
//	}
//
//	template<class V>
//	static IndexedTriangleList<V> make()
//	{
//		return makeTesselated<V>(24);
//	}
//};

class Sphere
{
public:
	static IndexedTriangleList makeTesselated(VertexLayout layout, int latDiv, int longDiv)
	{
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float lattitudeAngle = PI / (float) latDiv;
		const float longitudeAngle = 2.0f * PI / (float) longDiv;

		VertexBuffer vb{ std::move(layout) };
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = dx::XMVector3Transform(base, dx::XMMatrixRotationX(lattitudeAngle * (float) iLat));
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vec3f calculatedPos;
				auto v = dx::XMVector3Transform(latBase, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
				dx::XMStoreFloat3(&calculatedPos, v);
				vb.emplaceBack(calculatedPos);
			}
		}

		// add the cap vertices
		const auto iNorthPole = static_cast<ushort>(vb.size());
		vec3f northPos;
		dx::XMStoreFloat3(&northPos, base);
		vb.emplaceBack(northPos);
		
		const auto iSouthPole = static_cast<ushort>(vb.size());
		vec3f southPos;
		dx::XMStoreFloat3(&southPos, base);
		vb.emplaceBack(southPos);
		

		const auto calcIdx = [ latDiv, longDiv ] (ushort iLat, ushort iLong) {
				return iLat * longDiv + iLong;
			};
		list<ushort> indices;
		for (ushort iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (ushort iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (ushort iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(vb), std::move(indices) };
	}

	static IndexedTriangleList make(std::optional<VertexLayout> layout = std::nullopt)
	{
		if (!layout)
			layout = VertexLayout{}.append(AttributeType::POSITION3D);
		return makeTesselated(std::move(*layout), 12, 24);
	}
};

