#pragma once

#include "Object.h"
#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Frustum.h"
#include "Math/MathUtils.h"
#include "Updatable.h"

#include <initializer_list>
#include <string>

using oakvr::math::Vector3;
using oakvr::math::Matrix;

namespace oakvr
{
	namespace render
	{
		class Camera : public Updatable
		{
		public:	
			Camera() = delete;
			Camera(const std::string &, const Vector3 &pos, const Vector3 &target, const Vector3 &up);
			Camera(const std::string &, const std::initializer_list<Vector3> &);
			~Camera();

			auto Update(float dt)->bool override { return true; }

			auto GetPosition() const -> const Vector3 &{ return m_position; }
			auto GetForward() const -> const Vector3 &{ return m_forward; }
			auto GetUp() const -> const Vector3 &{ return m_up; }
			auto GetRight() const ->Vector3{ 
				return m_up.Cross(m_forward); }
			auto ComputeViewMatrix() const -> Matrix;
			auto GetProjMatrix() const -> Matrix { return m_matProj; }
			
			auto GetFrustum() const->Frustum;

			void SetPosition( const Vector3 & position )	{ m_position = position; }
			void SetLook(const Vector3& forward)			{ m_forward = forward; }
			void SetUp( const Vector3& up )					{ m_up = up; }
			void SetPerspectiveProjection(float fov, float aspectY, float near, float far) { m_matProj = Matrix::PerspectiveProjection(fov, aspectY, near, far); m_fov = fov; m_rwWidth = aspectY; m_rwHeight = 1; m_near = near; m_far = far; }
			void SetPerspectiveProjection(float fov, float width, float height, float near, float far) { m_matProj = Matrix::PerspectiveProjection(fov, width, height, near, far); m_fov = fov; m_rwWidth = width; m_rwHeight = height; m_near = near; m_far = far; }
			void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far) { m_matProj = Matrix::OrthographicProjection(left, right, bottom, top, near, far); m_fov = 0.f; m_rwWidth = right - left; m_rwHeight = top - bottom; m_near = near; m_far = far; }
			void SetOrthographicProjection(float width, float height, float near, float far) { m_matProj = Matrix::OrthographicProjection(-width / 2, width / 2, -height / 2, height / 2, near, far); m_fov = 0.f; m_rwWidth = width; m_rwHeight = height; m_near = near; m_far = far; }

			void Rotate( float alpha, float beta, float gamma );	// angles around x,y,z axes [radians]
			inline void Translate(float x, float y, float z) { Translate({ x, y, z }); }
			inline void Translate(const oakvr::math::Vector3 &displacement) { m_position += displacement; }

			void OnRenderWindowSizeChanged(float newWidth, float newHeight);

			const std::string &GetId() const { return m_id; }
			
		private:
			std::string m_id;

		protected:
			Vector3		m_position;
			Vector3		m_forward;
			Vector3		m_up;

			// projection config
			float m_fov = 0.f;
			float m_rwWidth = 0.f;
			float m_rwHeight = 0.f;
			float m_near = 0.f;
			float m_far = 0.f;

			Matrix	m_matProj;
		};
	} // namespace Render
}
