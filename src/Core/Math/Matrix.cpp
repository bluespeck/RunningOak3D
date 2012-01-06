
#include <cstring>
#include "Matrix.h"
#include <cmath>

namespace Oak3D
{
	namespace Math
	{

		// --------------------------------------------------------------------------------
		Matrix::Matrix()
		{

		}

		// --------------------------------------------------------------------------------
		Matrix::Matrix(float *a)
		{
			std::memcpy(m, a, sizeof(m));
		}		

		// --------------------------------------------------------------------------------
		void Matrix::SetInverseOfRotoTranslation()
		{
			Matrix mat;
			mat._11 = _11;
			mat._12 = _21;
			mat._13 = _31;
			mat._14 = -(_11 * _14 + _21 * _24 + _31 * _34);

			mat._21 = _12;
			mat._22 = _22;
			mat._23 = _32;
			mat._24 = -(_12 * _14 + _22 * _24 + _32 * _34);

			mat._31 = _13;
			mat._32 = _23;
			mat._33 = _33;
			mat._34 = -(_13 * _14 + _23 * _24 + _33 * _34);
			
			mat._41 = mat._42 = mat._43 = 0.0f; mat._44 = 1.0f;

			*this = mat;
		}

		// --------------------------------------------------------------------------------
		Matrix Matrix::CreateInverseOfRotoTranslation() const
		{
			Matrix mat;
			mat._11 = _11;
			mat._12 = _21;
			mat._13 = _31;
			mat._14 = -(_11 * _14 + _21 * _24 + _31 * _34);

			mat._21 = _12;
			mat._22 = _22;
			mat._23 = _32;
			mat._24 = -(_12 * _14 + _22 * _24 + _32 * _34);

			mat._31 = _13;
			mat._32 = _23;
			mat._33 = _33;
			mat._34 = -(_13 * _14 + _23 * _24 + _33 * _34);

			mat._41 = mat._42 = mat._43 = 0.0f; mat._44 = 1.0f;
			
			return mat;
		}

		// --------------------------------------------------------------------------------
		void Matrix::SetInverse()
		{
			//a11 = n(-h k + g l) + o( h j - f l) + p( - g j + f k)
			//a12 = n(d k - c l) + o(- d j + b l) + p(c j - b k)
			//a13 = n(-d g + c h) + o(d f - b h) + p(- c f + b g)
			//a14 = j(d g - c h) + k(- d f + b h) + l(c f - b g)
			//
			//a21 = m(h k - g l) + o(- h i + e l) + p(g i - e k)
			//a22 = m(-d k + c l) + o(d i - a l) + p(- c i + a k)
			//a23 = m(d g - c h) + o(- d e + a h) + p(c e - a g)
			//a24 = i(-d g + c h) + k(d e - a h) + l(- c e + a g)
			//
			//a31 = m(-h j + f l) + n(h i - e l) + p(-f i + e j)
			//a32 = m(d j - b l) + n(- d i + a l) + p(b i - a j)
			//a33 = m(-d f + b h) + n(d e - a h) +p(- b e + a f)
			//a34 = i(d f - b h) + j(- d e + a h) + l(b e - a f)
			//
			//a41 = m(g j - f k) + n(- g i + e k) + o(f i - e j)
			//a42 = m(-c j + b k) + n(c i - a k) + o(- b i + a j)
			//a43 = m(c f - b g) + n(- c e + a g) + o(b e - a f)
			//a44 = i(-c f + b g) + j(c e - a g) + k(- b e + a f)
			//
			//// divide aij by det(A)
			//			
			//det(A) = m(j(d g - c h) + k(- d f + b h) + l(c f - b g)) + 
			// n(i(-d g + c h + k(d e - a h) + l(- c e + a g)) + 
			// o(i(d f - b h) + j(- d e + a h) + l(b e - a f)) + 
			// p(i(-c f + b g) + j(c e - a g) + k(- b e + a f))

			// a = 11 b = 12 c = 13 d = 14
			// e = 21 f = 22 g = 23 h = 24
			// i = 31 j = 32 k = 33 l = 34
			// m = 41 n = 42 o = 43 p = 44

			const float af = _11 * _22;
			const float ag = _11 * _23;
			const float ah = _11 * _24;
			const float aj = _11 * _32;
			const float ak = _11 * _33;
			const float al = _11 * _34;		
			
			const float be = _12 * _21;
			const float bg = _12 * _23;
			const float bh = _12 * _24;
			const float bi = _12 * _31;
			const float bk = _12 * _33;
			const float bl = _12 * _34;
			
			const float ce = _13 * _21;
			const float cf = _13 * _22;
			const float ch = _13 * _24;
			const float ci = _13 * _31;
			const float cj = _13 * _32;
			const float cl = _13 * _34;

			const float de = _14 * _21;
			const float df = _14 * _22;
			const float dg = _14 * _23;
			const float di = _14 * _31;		
			const float dj = _14 * _32;
			const float dk = _14 * _33;
			
			const float ej = _21 * _32;
			const float ek = _21 * _33;
			const float el = _21 * _34;
			
			const float fi = _22 * _31;
			const float fk = _22 * _33;
			const float fl = _22 * _34;
			
			const float gi = _23 * _31;		
			const float gj = _23 * _32;			
			const float gl = _23 * _34;			
			
			const float hi = _24 * _31;		
			const float hj = _24 * _32;			
			const float hk = _24 * _33;		
			
			
			const float invDet = 1.0f / (
				_41 * (_32 * ( dg - ch) + _33 * (-df + bh) + _34 * ( cf - bg)) + 
				_42 * (_31 * (-dg + ch) + _33 * ( de - ah) + _34 * (-ce + ag)) + 
				_43 * (_31 * ( df - bh) + _32 * (-de + ah) + _34 * ( be - af)) + 
				_44 * (_31 * (-cf + bg) + _32 * ( ce - ag) + _33 * (-be + af)));
			
			Matrix inv;
			inv._11 = invDet * _42 * (-hk + gl) + _43 * ( hj - fl) + _44 * (-gj + fk);
			inv._12 = invDet * _42 * ( dk - cl) + _43 * (-dj + bl) + _44 * ( cj - bk);
			inv._13 = invDet * _42 * (-dg + ch) + _43 * ( df - bh) + _44 * (-cf + bg);
			inv._14 = invDet * _32 * ( dg - ch) + _33 * (-df + bh) + _34 * ( cf - bg);

			inv._21 = invDet * _41 * ( hk - gl) + _43 * (-hi + el) + _44 * ( gi - ek);
			inv._22 = invDet * _41 * (-dk + cl) + _43 * ( di - al) + _44 * (-ci + ak);
			inv._23 = invDet * _41 * ( dg - ch) + _43 * (-de + ah) + _44 * ( ce - ag);
			inv._24 = invDet * _31 * (-dg + ch) + _33 * ( de - ah) + _34 * (-ce + ag);

			inv._31 = invDet * _41 * (-hj + fl) + _42 * ( hi - el) + _44 * (-fi + ej);
			inv._32 = invDet * _41 * ( dj - bl) + _42 * (-di + al) + _44 * ( bi - aj);
			inv._33 = invDet * _41 * (-df + bh) + _42 * ( de - ah) + _44 * (-be + af);
			inv._34 = invDet * _31 * ( df - bh) + _32 * (-de + ah) + _34 * ( be - af);

			inv._41 = invDet * _41 * ( gj - fk) + _42 * (-gi + ek) + _43 * ( fi - ej);
			inv._42 = invDet * _41 * (-cj + bk) + _42 * ( ci - ak) + _43 * (-bi + aj);
			inv._43 = invDet * _41 * ( cf - bg) + _42 * (-ce + ag) + _43 * ( be - af);
			inv._44 = invDet * _31 * (-cf + bg) + _32 * ( ce - ag) + _33 * (-be + af);
			
			*this = inv;
		}

		// --------------------------------------------------------------------------------
		Matrix Matrix::CreateInverse() const
		{
			const float af = _11 * _22;
			const float ag = _11 * _23;
			const float ah = _11 * _24;
			const float aj = _11 * _32;
			const float ak = _11 * _33;
			const float al = _11 * _34;		

			const float be = _12 * _21;
			const float bg = _12 * _23;
			const float bh = _12 * _24;
			const float bi = _12 * _31;
			const float bk = _12 * _33;
			const float bl = _12 * _34;

			const float ce = _13 * _21;
			const float cf = _13 * _22;
			const float ch = _13 * _24;
			const float ci = _13 * _31;
			const float cj = _13 * _32;
			const float cl = _13 * _34;

			const float de = _14 * _21;
			const float df = _14 * _22;
			const float dg = _14 * _23;
			const float di = _14 * _31;		
			const float dj = _14 * _32;
			const float dk = _14 * _33;

			const float ej = _21 * _32;
			const float ek = _21 * _33;
			const float el = _21 * _34;

			const float fi = _22 * _31;
			const float fk = _22 * _33;
			const float fl = _22 * _34;

			const float gi = _23 * _31;		
			const float gj = _23 * _32;			
			const float gl = _23 * _34;			

			const float hi = _24 * _31;		
			const float hj = _24 * _32;			
			const float hk = _24 * _33;		


			const float invDet = 1.0f / (
				_41 * (_32 * ( dg - ch) + _33 * (-df + bh) + _34 * ( cf - bg)) + 
				_42 * (_31 * (-dg + ch) + _33 * ( de - ah) + _34 * (-ce + ag)) + 
				_43 * (_31 * ( df - bh) + _32 * (-de + ah) + _34 * ( be - af)) + 
				_44 * (_31 * (-cf + bg) + _32 * ( ce - ag) + _33 * (-be + af)));

			Matrix inv;
			inv._11 = invDet * _42 * (-hk + gl) + _43 * ( hj - fl) + _44 * (-gj + fk);
			inv._12 = invDet * _42 * ( dk - cl) + _43 * (-dj + bl) + _44 * ( cj - bk);
			inv._13 = invDet * _42 * (-dg + ch) + _43 * ( df - bh) + _44 * (-cf + bg);
			inv._14 = invDet * _32 * ( dg - ch) + _33 * (-df + bh) + _34 * ( cf - bg);

			inv._21 = invDet * _41 * ( hk - gl) + _43 * (-hi + el) + _44 * ( gi - ek);
			inv._22 = invDet * _41 * (-dk + cl) + _43 * ( di - al) + _44 * (-ci + ak);
			inv._23 = invDet * _41 * ( dg - ch) + _43 * (-de + ah) + _44 * ( ce - ag);
			inv._24 = invDet * _31 * (-dg + ch) + _33 * ( de - ah) + _34 * (-ce + ag);

			inv._31 = invDet * _41 * (-hj + fl) + _42 * ( hi - el) + _44 * (-fi + ej);
			inv._32 = invDet * _41 * ( dj - bl) + _42 * (-di + al) + _44 * ( bi - aj);
			inv._33 = invDet * _41 * (-df + bh) + _42 * ( de - ah) + _44 * (-be + af);
			inv._34 = invDet * _31 * ( df - bh) + _32 * (-de + ah) + _34 * ( be - af);

			inv._41 = invDet * _41 * ( gj - fk) + _42 * (-gi + ek) + _43 * ( fi - ej);
			inv._42 = invDet * _41 * (-cj + bk) + _42 * ( ci - ak) + _43 * (-bi + aj);
			inv._43 = invDet * _41 * ( cf - bg) + _42 * (-ce + ag) + _43 * ( be - af);
			inv._44 = invDet * _31 * (-cf + bg) + _32 * ( ce - ag) + _33 * (-be + af);

			return inv;
		}

		// --------------------------------------------------------------------------------
		Matrix Matrix::CreateIdentityMatrix()
		{
			Matrix mat;
			mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
			mat._12 = mat._13 = mat._14 = 0.0f;
			mat._21 = mat._23 = mat._24 = 0.0f;
			mat._31 = mat._32 = mat._34 = 0.0f;
			mat._41 = mat._42 = mat._43 = 0.0f;
			return mat;
		}

		// --------------------------------------------------------------------------------
		void Matrix::SetYawPitchRoll(float yaw, float pitch, float roll)
		{
			float calpha = std::cos(yaw);
			float cbeta = std::cos(pitch);
			float cgamma = std::cos(roll);
			float salpha = std::sin(yaw);
			float sbeta = std::sin(pitch);
			float sgamma = std::sin(roll);
			
			_11 = calpha * cbeta;
			_12 = calpha * sbeta * sgamma - salpha * cgamma;
			_13 = calpha * sbeta * cgamma + salpha * sgamma;
			_21 = salpha * cbeta;
			_22 = salpha * sbeta * sgamma + calpha * cgamma;
			_23 = salpha * sbeta * cgamma - calpha * sgamma;
			_31 = -sbeta;
			_32 = cbeta * sgamma;
			_33 = cbeta * cgamma;
		}


		// --------------------------------------------------------------------------------
		Matrix Matrix::CreateYawPitchRoll(float yaw, float pitch, float roll)
		{
			float calpha = std::cos(yaw);
			float cbeta = std::cos(pitch);
			float cgamma = std::cos(roll);
			float salpha = std::sin(yaw);
			float sbeta = std::sin(pitch);
			float sgamma = std::sin(roll);

			Matrix mat;

			mat._11 = calpha * cbeta;
			mat._12 = calpha * sbeta * sgamma - salpha * cgamma;
			mat._13 = calpha * sbeta * cgamma + salpha * sgamma;
			mat._14 = 0.0f;
			mat._21 = salpha * cbeta;
			mat._22 = salpha * sbeta * sgamma + calpha * cgamma;
			mat._23 = salpha * sbeta * cgamma - calpha * sgamma;
			mat._24 = mat._24 = 0.0f;
			mat._31 = -sbeta;
			mat._32 = cbeta * sgamma;
			mat._33 = cbeta * cgamma;
			mat._34 = 0.0f;
			mat._41 = mat._42 = mat._43 = 0.0f;
			mat._44 = 1.0f;

			return mat;
		}

		// --------------------------------------------------------------------------------
		Matrix Matrix::operator+(const Matrix &mat) const
		{
			Matrix result;
			result._11 = _11 + mat._11;
			result._12 = _12 + mat._12;
			result._13 = _13 + mat._13;
			result._14 = _14 + mat._14;

			result._21 = _21 + mat._21;
			result._22 = _22 + mat._22;
			result._23 = _23 + mat._23;
			result._24 = _24 + mat._24;

			result._31 = _31 + mat._31;
			result._32 = _32 + mat._32;
			result._33 = _33 + mat._33;
			result._34 = _34 + mat._34;

			result._41 = _41 + mat._41;
			result._42 = _42 + mat._42;
			result._43 = _43 + mat._43;
			result._44 = _44 + mat._44;

			return result;
		}

		// --------------------------------------------------------------------------------
		Matrix Matrix::operator*(const Matrix &mat) const
		{
			Matrix result;
			result._11 = _11 * mat._11 + _12 * mat._21 + _13 * mat._31 + _14 * mat._41;
			result._12 = _11 * mat._12 + _12 * mat._22 + _13 * mat._32 + _14 * mat._42;
			result._13 = _11 * mat._13 + _12 * mat._23 + _13 * mat._33 + _14 * mat._43;
			result._14 = _11 * mat._14 + _12 * mat._24 + _13 * mat._34 + _14 * mat._44;

			result._21 = _21 * mat._11 + _22 * mat._21 + _23 * mat._31 + _24 * mat._41;
			result._22 = _21 * mat._12 + _22 * mat._22 + _23 * mat._32 + _24 * mat._42;
			result._23 = _21 * mat._13 + _22 * mat._23 + _23 * mat._33 + _24 * mat._43;
			result._24 = _21 * mat._14 + _22 * mat._24 + _23 * mat._34 + _24 * mat._44;

			result._31 = _31 * mat._11 + _32 * mat._21 + _33 * mat._31 + _34 * mat._41;
			result._32 = _31 * mat._12 + _32 * mat._22 + _33 * mat._32 + _34 * mat._42;
			result._33 = _31 * mat._13 + _32 * mat._23 + _33 * mat._33 + _34 * mat._43;
			result._34 = _31 * mat._14 + _32 * mat._24 + _33 * mat._34 + _34 * mat._44;

			result._41 = _41 * mat._11 + _42 * mat._21 + _43 * mat._31 + _44 * mat._41;
			result._42 = _41 * mat._12 + _42 * mat._22 + _43 * mat._32 + _44 * mat._42;
			result._43 = _41 * mat._13 + _42 * mat._23 + _43 * mat._33 + _44 * mat._43;
			result._44 = _41 * mat._14 + _42 * mat._24 + _43 * mat._34 + _44 * mat._44;
			
			return result;
		}

	} // namespace Math
} // namespace Oak3D