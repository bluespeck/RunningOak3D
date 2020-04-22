#pragma once

#include "Utils/Types.h"
#include "Mesh.h"
#include "Math/Matrix44.h"

namespace oakvr::render
{
	class Renderable
	{
	public:
		Renderable(const MeshSharedPointer &pMesh = nullptr);
		~Renderable() {}

		auto SetMesh(const MeshSharedPointer &pMesh) -> void;
		auto GetMesh() const -> const MeshSharedPointer &;

		auto SetTransform(const oakvr::math::Matrix44 &transform) -> void;
		auto GetTransform() const -> const oakvr::math::Matrix44 &;
		auto Transform(const oakvr::math::Matrix44 &transform) -> void;

	private:
		MeshSharedPointer m_mesh;
		oakvr::math::Matrix44 m_transform;
	};
}