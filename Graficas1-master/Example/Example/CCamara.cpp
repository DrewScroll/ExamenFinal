#include "CCamara.h"

#define M_PI 3.141592654

const	VECTOR4D	CCamara::LookConstCameraSpace = { 0.0f, 0.0f, 1.0f, 0.0f };
const	VECTOR4D	CCamara::RightConstCameraSpace = { 1.0f, 0.0f, 0.0f, 0.0f };
const	VECTOR4D	CCamara::UpConstCameraSpace = { 0.0f, 1.0f, 0.0f, 0.0f };

CCamara::CCamara() {
	Reset();
}

void	CCamara::Init(VECTOR4D position, float fov, float ratio, float np, float fp, bool lf) {
	this->Fov = fov;
	this->AspectRatio = ratio;
	this->NPlane = np;
	this->FPlane = fp;
	this->Eye = position;
	this->LeftHanded = lf;
	CreatePojection();
}

void	CCamara::CreatePojection() {
	if (LeftHanded)
		Projection = PerspectiveFOVLH(Fov, AspectRatio, NPlane, FPlane);
	else
		Projection = PerspectiveFOVRH(Fov, AspectRatio, NPlane, FPlane);
}

void	CCamara::SetLookAt(VECTOR4D v) {
	Look = v - Eye;
	Look = Normalize(Look);
	Pitch = atan2f(-Look.y, -Look.z);
	Yaw = atan2f(-Look.x, Look.z);

	Update(1.0f / 60.0f);
}

void	CCamara::MoveForward(float dt) {
	Velocity.z += Speed * dt;
}

void	CCamara::MoveBackward(float dt) {
	Velocity.z -= Speed * dt;
}

void	CCamara::StrafeLeft(float dt) {
	Velocity.x -= Speed * dt;
}

void	CCamara::StrafeRight(float dt) {
	Velocity.x += Speed * dt;
}

void	CCamara::SetFov(float f) {
	this->Fov = f;
	CreatePojection();
}

void	CCamara::SetRatio(float r) {
	this->AspectRatio = r;
	CreatePojection();
}

void	CCamara::SetPlanes(float n, float f) {
	this->NPlane = n;
	this->FPlane = f;
	CreatePojection();
}

void	CCamara::MoveYaw(float f) {
	if (MaxYaw != 0.0) {
		if ((Yaw + f) > MaxYaw || (Yaw + f) < -MaxYaw)
			return;
	}
	Yaw += f;
}

void	CCamara::MovePitch(float f) {
	if (MaxPitch != 0.0) {
		if ((Pitch + f) > MaxPitch || (Pitch + f) < -MaxPitch)
			return;
	}
	Pitch += f;
}

void	CCamara::MoveRoll(float f) {
	if (MaxRoll != 0.0) {
		if ((Roll + f) > MaxRoll || (Roll + f) < -MaxRoll)
			return;
	}
	Roll += f;
}

void	CCamara::Update(float dt) {
	MATRIX4D	X_, Y_, Z_, T_;
	X_ = RotationXLH(-Pitch);
	Y_ = RotationYLH(-Yaw);
	Z_ = RotationZLH(-Roll);
	View = Z_*Y_*X_;

	MATRIX4D transpose;
	transpose = Transpose(View);
	Look = TransformNormalLH(LookConstCameraSpace, transpose);
	Up = TransformNormalLH(UpConstCameraSpace, transpose);
	Right = TransformNormalLH(RightConstCameraSpace, transpose);

	Look = Normalize(Look);
	Up = Normalize(Up);
	Right = Normalize(Right);

	VECTOR4D currentvelocity = Right*Velocity.x + Up*Velocity.y + Look*Velocity.z;
	Velocity = Velocity - Velocity*Friction;
	Eye = Eye + currentvelocity;

	VECTOR4D TEYE = Eye*-1;
	T_ = TranslationLH(TEYE.x, TEYE.y, TEYE.z);
	View = T_*View;
	VP = View*Projection;
}

void	CCamara::Reset() {
	Eye = { 0.0f, 0.0f, 0.0f };
	Velocity = { 0.0f, 0.0f, 0.0f };
	Fov = 45.0f * (M_PI / 180.0f);
	NPlane = 0.01f;
	FPlane = 1000.0f;
	AspectRatio = 1.0f;
	Speed = 5000.0;
	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
	Friction = 0.1f;
	MaxRoll = 0.0f;
	MaxPitch = 89.0f*(M_PI / 180.0f);
	MaxYaw = 0.0f;
	LeftHanded = true;
}