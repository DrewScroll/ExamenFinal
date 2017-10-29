#include "Matrix4D.h"
#include <string.h>
#include <math.h>

#define M_PI 3.141592654

MATRIX4D Zero()
{
	MATRIX4D Z;
	memset(&Z, 0, sizeof(MATRIX4D));
	return Z;
}

MATRIX4D operator*(MATRIX4D & A, MATRIX4D & B)
{
	MATRIX4D R = Zero();
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < 4; k++)
				R.m[j][i] += A.m[j][k] * B.m[k][i];
	return R;
}

VECTOR4D operator*(MATRIX4D & A, VECTOR4D & V)
{
	VECTOR4D R = { 0, 0, 0, 0 };
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			R.v[j] += A.m[j][i] * V.v[i];
	return R;
}

VECTOR4D operator*(VECTOR4D & V, MATRIX4D & A)
{
	VECTOR4D R = { 0, 0, 0, 0 };
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			R.v[j] += A.m[i][j] * V.v[i];
	return R;
}

VECTOR4D operator*(VECTOR4D & A, VECTOR4D & B)
{
	VECTOR4D R = { A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w };
	return R;
}

VECTOR4D operator-(VECTOR4D & A, VECTOR4D & B)
{
	VECTOR4D R = { A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w };
	return R;
}

VECTOR4D operator+(VECTOR4D& A, VECTOR4D& B)
{
	VECTOR4D R = { A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w };
	return R;
}

VECTOR4D operator*(VECTOR4D& A, float s)
{
	VECTOR4D R = { A.x*s, A.y*s, A.z*s, A.w*s };
	return R;
}

VECTOR4D operator-=(VECTOR4D & A, VECTOR4D & B)
{
	VECTOR4D R = { A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w };
	return R;
}

float Dot(VECTOR4D & A, VECTOR4D & B)
{
	return  A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
}

float Magnity(VECTOR4D & A)
{
	return sqrtf(Dot(A, A));
}

VECTOR4D Normalize(VECTOR4D & A)
{
	float inv = 1.0f / Magnity(A);
	VECTOR4D R = { A.x*inv, A.y*inv, A.z*inv, A.w*inv };
	return R;
}

MATRIX4D Identity()
{
	MATRIX4D I;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			I.m[j][i] = (i == j) ? 1.0f : 0.0f;
	return I;
}

MATRIX4D Transpose(MATRIX4D & M)
{
	MATRIX4D R;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			R.m[i][j] = M.m[j][i];
	return R;

}

MATRIX4D FastInverse(MATRIX4D & M)
{
	MATRIX4D R;
	VECTOR4D InvPos;
	R = Transpose(M);
	InvPos = R.vec[3];
	R.m03 = -Dot(R.vec[0], InvPos);
	R.m13 = -Dot(R.vec[1], InvPos);
	R.m23 = -Dot(R.vec[2], InvPos);
	R.vec[3].x = 0;
	R.vec[3].y = 0;
	R.vec[3].z = 0;
	return R;
}


MATRIX4D TranslationLH(float dx, float dy, float dz)
{
	MATRIX4D T = Identity();
	T.m30 = dx;
	T.m31 = dy;
	T.m32 = dz;
	return T;
}

MATRIX4D TranslationRH(float dx, float dy, float dz)
{
	MATRIX4D T = Identity();
	T.m03 = dx;
	T.m13 = dy;
	T.m23 = dz;
	return T;
}

MATRIX4D RotationXRH(float theta)
{
	MATRIX4D R = Identity();
	R.m22 = R.m11 = cosf(theta);
	R.m12 = -sinf(theta);
	R.m21 = -R.m12;
	return R;
}

MATRIX4D RotationYRH(float theta)
{
	MATRIX4D R = Identity();
	R.m00 = R.m22 = cosf(theta);
	R.m20 = -sinf(theta);
	R.m02 = -R.m20;
	return R;
}

MATRIX4D RotationZRH(float theta)
{
	MATRIX4D R = Identity();
	R.m11 = R.m00 = cosf(theta);
	R.m01 = -sinf(theta);
	R.m10 = -R.m01;
	return R;
}

MATRIX4D RotationXLH(float theta)
{
	MATRIX4D R = Identity();
	R.m22 = R.m11 = cosf(theta);
	R.m12 = sinf(theta);
	R.m21 = -R.m12;
	return R;
}

MATRIX4D RotationYLH(float theta)
{
	MATRIX4D R = Identity();
	R.m00 = R.m22 = cosf(theta);
	R.m20 = sinf(theta);
	R.m02 = -R.m20;
	return R;
}

MATRIX4D RotationZLH(float theta)
{
	MATRIX4D R = Identity();
	R.m11 = R.m00 = cosf(theta);
	R.m01 = sinf(theta);
	R.m10 = -R.m01;
	return R;
}

MATRIX4D Scaling(float sx, float sy, float sz)
{
	MATRIX4D S = Identity();
	S.m00 = sx;
	S.m11 = sy;
	S.m22 = sz;
	return S;
}

VECTOR4D Cross3(VECTOR4D & A, VECTOR4D & B)
{
	VECTOR4D R;
	R.x = A.y*B.z - A.z*B.y;
	R.y = A.z*B.x - A.x*B.z;
	R.z = A.x*B.y - A.y*B.x;
	R.w = 0;
	return R;
}

MATRIX4D LookAtRH(VECTOR4D & EyePos, VECTOR4D & Target, VECTOR4D & Up)
{
	VECTOR4D xDir, yDir, zDir;
	zDir = Normalize(EyePos - Target);
	xDir = Normalize(Cross3(Up, zDir));
	yDir = Cross3(zDir, xDir);
	MATRIX4D View = {	 xDir.x,			  yDir.x,			  zDir.x,		 0,
						 xDir.y,			  yDir.y,			  zDir.y,		 0,
						 xDir.z,			  yDir.z,			  zDir.z,		 0,
					-Dot(xDir,EyePos), -Dot(yDir, EyePos), -Dot(zDir, EyePos),   1 };
	return View;
}

MATRIX4D LookAtLH(VECTOR4D & EyePos, VECTOR4D & Target, VECTOR4D & Up)
{
	VECTOR4D xDir, yDir, zDir;
	zDir = Normalize(Target - EyePos);
	xDir = Normalize(Cross3(Up, zDir));
	yDir = Cross3(zDir, xDir);
	MATRIX4D View = { xDir.x,			yDir.x,				zDir.x,		   0,
					  xDir.y,			yDir.y,				zDir.y,		   0,
					  xDir.z,			yDir.z,				zDir.z,		   0,
				-Dot(xDir,EyePos), -Dot(yDir, EyePos), -Dot(zDir, EyePos), 1 };
	return View;
}

MATRIX4D OrthoRH(float width, float height, float zNear, float zFar)
{
	MATRIX4D O = { 2 / width,	    0,				 0,			    0,
					  0,		2 / height,			 0,			    0,
					  0,		    0,		  1 / (zNear - zFar),   0,
					  0,		    0,		zNear / (zNear - zFar),	1 };
	return O;
}

MATRIX4D OrthoLH(float width, float height, float zNear, float zFar)
{
	MATRIX4D O = { 2 / width,		0,				  0,		    0,
					  0,		2 / height,			  0,		    0,
					  0,			0,		  1 / (zFar - zNear),   0,
					  0,			0,		zNear / (zNear - zFar),	1 };
	return O;
}

MATRIX4D PerspectiveFOVRH(float FOVY, float ratio, float zNear, float zFar)
{
	float ang = FOVY / 2;

	float h = cos(ang) / sin(ang);
	float w = h / ratio;
	MATRIX4D P = { w,	0,				 0,				 0,
				   0,	h,				 0,				 0,
				   0,	0,	   zFar / (zNear - zFar),   -1,
				   0,	0,	zNear*zFar / (zNear - zFar), 0 };
	return P;
}

MATRIX4D PerspectiveFOVLH(float FOVY, float ratio, float zNear, float zFar)
{
	float ang = FOVY / 2;

	float h = cos(ang) / sin(ang);
	float w = h / ratio;
	MATRIX4D P = { w,	0,				  0,				0,
				   0,	h,				  0,				0,
				   0,	0,		zFar / (zFar - zNear),		1,
				   0,	0,	-zNear*zFar / (zFar - zNear),	0 };
	return P;
}


float Inverse(MATRIX4D & M, MATRIX4D & R)
{
	double inv[16], det;
	int i;

	inv[0] = M.v[5] * M.v[10] * M.v[15] -
		M.v[5] * M.v[11] * M.v[14] -
		M.v[9] * M.v[6] * M.v[15] +
		M.v[9] * M.v[7] * M.v[14] +
		M.v[13] * M.v[6] * M.v[11] -
		M.v[13] * M.v[7] * M.v[10];

	inv[4] = -M.v[4] * M.v[10] * M.v[15] +
		M.v[4] * M.v[11] * M.v[14] +
		M.v[8] * M.v[6] * M.v[15] -
		M.v[8] * M.v[7] * M.v[14] -
		M.v[12] * M.v[6] * M.v[11] +
		M.v[12] * M.v[7] * M.v[10];

	inv[8] = M.v[4] * M.v[9] * M.v[15] -
		M.v[4] * M.v[11] * M.v[13] -
		M.v[8] * M.v[5] * M.v[15] +
		M.v[8] * M.v[7] * M.v[13] +
		M.v[12] * M.v[5] * M.v[11] -
		M.v[12] * M.v[7] * M.v[9];

	inv[12] = -M.v[4] * M.v[9] * M.v[14] +
		M.v[4] * M.v[10] * M.v[13] +
		M.v[8] * M.v[5] * M.v[14] -
		M.v[8] * M.v[6] * M.v[13] -
		M.v[12] * M.v[5] * M.v[10] +
		M.v[12] * M.v[6] * M.v[9];

	inv[1] = -M.v[1] * M.v[10] * M.v[15] +
		M.v[1] * M.v[11] * M.v[14] +
		M.v[9] * M.v[2] * M.v[15] -
		M.v[9] * M.v[3] * M.v[14] -
		M.v[13] * M.v[2] * M.v[11] +
		M.v[13] * M.v[3] * M.v[10];

	inv[5] = M.v[0] * M.v[10] * M.v[15] -
		M.v[0] * M.v[11] * M.v[14] -
		M.v[8] * M.v[2] * M.v[15] +
		M.v[8] * M.v[3] * M.v[14] +
		M.v[12] * M.v[2] * M.v[11] -
		M.v[12] * M.v[3] * M.v[10];

	inv[9] = -M.v[0] * M.v[9] * M.v[15] +
		M.v[0] * M.v[11] * M.v[13] +
		M.v[8] * M.v[1] * M.v[15] -
		M.v[8] * M.v[3] * M.v[13] -
		M.v[12] * M.v[1] * M.v[11] +
		M.v[12] * M.v[3] * M.v[9];

	inv[13] = M.v[0] * M.v[9] * M.v[14] -
		M.v[0] * M.v[10] * M.v[13] -
		M.v[8] * M.v[1] * M.v[14] +
		M.v[8] * M.v[2] * M.v[13] +
		M.v[12] * M.v[1] * M.v[10] -
		M.v[12] * M.v[2] * M.v[9];

	inv[2] = M.v[1] * M.v[6] * M.v[15] -
		M.v[1] * M.v[7] * M.v[14] -
		M.v[5] * M.v[2] * M.v[15] +
		M.v[5] * M.v[3] * M.v[14] +
		M.v[13] * M.v[2] * M.v[7] -
		M.v[13] * M.v[3] * M.v[6];

	inv[6] = -M.v[0] * M.v[6] * M.v[15] +
		M.v[0] * M.v[7] * M.v[14] +
		M.v[4] * M.v[2] * M.v[15] -
		M.v[4] * M.v[3] * M.v[14] -
		M.v[12] * M.v[2] * M.v[7] +
		M.v[12] * M.v[3] * M.v[6];

	inv[10] = M.v[0] * M.v[5] * M.v[15] -
		M.v[0] * M.v[7] * M.v[13] -
		M.v[4] * M.v[1] * M.v[15] +
		M.v[4] * M.v[3] * M.v[13] +
		M.v[12] * M.v[1] * M.v[7] -
		M.v[12] * M.v[3] * M.v[5];

	inv[14] = -M.v[0] * M.v[5] * M.v[14] +
		M.v[0] * M.v[6] * M.v[13] +
		M.v[4] * M.v[1] * M.v[14] -
		M.v[4] * M.v[2] * M.v[13] -
		M.v[12] * M.v[1] * M.v[6] +
		M.v[12] * M.v[2] * M.v[5];

	inv[3] = -M.v[1] * M.v[6] * M.v[11] +
		M.v[1] * M.v[7] * M.v[10] +
		M.v[5] * M.v[2] * M.v[11] -
		M.v[5] * M.v[3] * M.v[10] -
		M.v[9] * M.v[2] * M.v[7] +
		M.v[9] * M.v[3] * M.v[6];

	inv[7] = M.v[0] * M.v[6] * M.v[11] -
		M.v[0] * M.v[7] * M.v[10] -
		M.v[4] * M.v[2] * M.v[11] +
		M.v[4] * M.v[3] * M.v[10] +
		M.v[8] * M.v[2] * M.v[7] -
		M.v[8] * M.v[3] * M.v[6];

	inv[11] = -M.v[0] * M.v[5] * M.v[11] +
		M.v[0] * M.v[7] * M.v[9] +
		M.v[4] * M.v[1] * M.v[11] -
		M.v[4] * M.v[3] * M.v[9] -
		M.v[8] * M.v[1] * M.v[7] +
		M.v[8] * M.v[3] * M.v[5];

	inv[15] = M.v[0] * M.v[5] * M.v[10] -
		M.v[0] * M.v[6] * M.v[9] -
		M.v[4] * M.v[1] * M.v[10] +
		M.v[4] * M.v[2] * M.v[9] +
		M.v[8] * M.v[1] * M.v[6] -
		M.v[8] * M.v[2] * M.v[5];

	det = M.v[0] * inv[0] + M.v[1] * inv[4] + M.v[2] * inv[8] + M.v[3] * inv[12];

	if (fabs(det) < 0.0001)
		return 0.0f;

	double invdet = 1.0 / det;

	for (i = 0; i < 16; i++)
		R.v[i] = (float)(inv[i] * invdet);

	return (float)det;
}

VECTOR4D TransformNormalLH(const VECTOR4D &v, const MATRIX4D &mat) {
	VECTOR4D vpout;
	vpout.x = v.x*mat.m[0][0] + v.y*mat.m[1][0] + v.z*mat.m[2][0];
	vpout.y = v.x*mat.m[0][1] + v.y*mat.m[1][1] + v.z*mat.m[2][1];
	vpout.z = v.x*mat.m[0][2] + v.y*mat.m[1][2] + v.z*mat.m[2][2];
	vpout.w = 1.0f;
	return vpout;
}
VECTOR4D TransformNormalRH(const VECTOR4D &v, const MATRIX4D &mat) {
	VECTOR4D vpout;
	vpout.x = v.x*mat.m[0][0] + v.y*mat.m[0][1] + v.z*mat.m[0][2];
	vpout.y = v.x*mat.m[1][0] + v.y*mat.m[1][1] + v.z*mat.m[1][2];
	vpout.z = v.x*mat.m[2][0] + v.y*mat.m[2][1] + v.z*mat.m[2][2];
	vpout.w = 1.0f;
	return vpout;
}

MATRIX4D MatTranslation(VECTOR4D & v)
{
	MATRIX4D M;
	M.m[3][0] = v.x;
	M.m[3][1] = v.y;
	M.m[3][2] = v.z;
	M.m[3][3] = 1.0f;
	return M;
}

//MATRIX4D &MATRIX4D::operator=(float * t)
//{
//	for (int i = 0; i < 16; i++)
//	{
//		this->v[i] = t[i];
//	}
//	return *this;
//}
