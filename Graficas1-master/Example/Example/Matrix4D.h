#pragma once

//#include "PrimitiveBase.h"

struct VECTOR4D
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct 
		{
			float r, g, b, a;
		};
		struct 
		{
			unsigned long ulx, uly, ulz, ulw;
		};
		float v[4];
	};
};

struct MATRIX4D
{
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		float m[4][4];
		VECTOR4D vec[4];
		float v[16];
	};
};

MATRIX4D Zero();
MATRIX4D operator*(MATRIX4D& A, MATRIX4D &B);
VECTOR4D operator*(MATRIX4D& A, VECTOR4D& V);
VECTOR4D operator*(VECTOR4D& V, MATRIX4D& A);
VECTOR4D operator*(VECTOR4D& A, VECTOR4D& B);
VECTOR4D operator-(VECTOR4D& A, VECTOR4D &B);
VECTOR4D operator+(VECTOR4D& A, VECTOR4D &B);
VECTOR4D operator*(VECTOR4D& A, float s);
float    Dot(VECTOR4D& A, VECTOR4D& B);
float    Magnity(VECTOR4D& A);
VECTOR4D Normalize(VECTOR4D& A);
MATRIX4D Identity();
MATRIX4D Transpose(MATRIX4D& M);
MATRIX4D FastInverse(MATRIX4D& M);
MATRIX4D TranslationLH(float dx, float dy, float dz);
MATRIX4D TranslationRH(float dx, float dy, float dz);
MATRIX4D RotationXRH(float theta);
MATRIX4D RotationYRH(float theta);
MATRIX4D RotationZRH(float theta);
MATRIX4D RotationXLH(float theta);
MATRIX4D RotationYLH(float theta);
MATRIX4D RotationZLH(float theta);
MATRIX4D Scaling(float sx, float sy, float sz);
VECTOR4D Cross3(VECTOR4D&A, VECTOR4D &B);
MATRIX4D LookAtRH(VECTOR4D& EyePos, VECTOR4D& Target, VECTOR4D& Up);
MATRIX4D LookAtLH(VECTOR4D& EyePos, VECTOR4D& Target, VECTOR4D& Up);
MATRIX4D OrthoRH(float width, float height, float zNear, float zFar);
MATRIX4D OrthoLH(float width, float height, float zNear, float zFar);
MATRIX4D PerspectiveFOVRH(float FOVY, float ratio, float zNear, float zFar);
MATRIX4D PerspectiveFOVLH(float FOVY, float ratio, float zNear, float zFar);
float Inverse(MATRIX4D & M, MATRIX4D & R);
VECTOR4D TransformNormalLH(const VECTOR4D& v,const MATRIX4D& M);
VECTOR4D TransformNormalRH(const VECTOR4D& v,const MATRIX4D& M);