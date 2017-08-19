#ifndef T800_CAMERA_H
#define T800_CAMERA_H

#include "Matrix4D.h"
#include <math.h>

class CCamara {
public:
	CCamara();

	void	Init(VECTOR4D position, float fov, float ratio, float np, float fp, bool lf = true);
	void	CreatePojection();

	void	SetLookAt(VECTOR4D v);

	void	MoveForward(float dt);
	void	MoveBackward(float dt);
	void	StrafeLeft(float dt);
	void	StrafeRight(float dt);

	void	MoveYaw(float f);
	void	MovePitch(float f);
	void	MoveRoll(float f);

	void	Update(float dt);
	void	Reset();

	void	SetFov(float f);
	void	SetRatio(float r);
	void	SetPlanes(float n, float f);

	float		Fov;
	float		AspectRatio;
	float		NPlane;
	float		FPlane;

	float		Yaw;
	float		Pitch;
	float		Roll;

	float		MaxRoll;
	float		MaxPitch;
	float		MaxYaw;

	float		Speed;
	float		Friction;

	bool		LeftHanded;

	VECTOR4D	Eye;
	VECTOR4D	Look;
	VECTOR4D	Right;
	VECTOR4D	Up;

	VECTOR4D	Velocity;

	MATRIX4D	Position;
	MATRIX4D	RotX;
	MATRIX4D	RotY;
	MATRIX4D	RotZ;

	MATRIX4D	View;
	MATRIX4D	Projection;
	MATRIX4D	VP;

	static const	VECTOR4D	LookConstCameraSpace;
	static const	VECTOR4D	RightConstCameraSpace;
	static const	VECTOR4D	UpConstCameraSpace;
};


#endif