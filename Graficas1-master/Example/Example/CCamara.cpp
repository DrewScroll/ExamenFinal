#include "CCamara.h"
#include <math.h>

const VECTOR4D CCamara::LookConstCameraSpace = { 0.0f, 0.0f, 1.0f };
const VECTOR4D CCamara::RIghtConstCameraSPace = { 1.0f, 0.0f, 0.0f };
const VECTOR4D CCamara::UpConstCameraSpace = { 0.0f, 1.0f, 0.0f };

CCamara::CCamara()
{
	Reset();
}


CCamara::~CCamara()
{
}

void CCamara::Init(VECTOR4D Pos, float FOV, float Ratio, float NP, float FP, bool LF)
{
	this->Eye = Pos;
	this->FOV = FOV;
	this->AR = Ratio;
	this->NearPlane = NP;
	this->FarPlane = FP;
	this->LeftHanded = LF;

	CreateProjection();
}

void CCamara::CreateProjection()
{
	if (LeftHanded)
	{
		Projection = PerspectiveFOVLH(FOV, AR, NearPlane, FarPlane);
	}
	else
	{
		Projection = PerspectiveFOVRH(FOV, AR, NearPlane, FarPlane);
	}
}

void CCamara::SetLookAt(VECTOR4D v)
{
	Look = v - Eye;
	Normalize(Look);
	Pitch = atan2f(-Look.y, -Look.z);
	Yaw = atan2f(-Look.x, Look.z);

	Update(1.0f / 60.0f);
}

void CCamara::MoveFront(float DT)
{
	Velocity.z += Speed * DT;
}

void CCamara::MoveBack(float DT)
{
	Velocity.z -= Speed * DT;
}

void CCamara::MoveLeft(float DT)
{
	Velocity.x -= Speed * DT;
}

void CCamara::MoveRight(float DT)
{
	Velocity.x += Speed * DT;
}

void CCamara::MoveYaw(float f)
{
	if (MaxYaw != 0.0f)
	{
		if ((Yaw + f) > MaxYaw || (Yaw + f) < -MaxYaw)
		{
			return;
		}
	}
	Yaw += f;
}

void CCamara::MovePitch(float f)
{
	if (MaxPitch != 0.0f)
	{
		if ((Pitch + f) > MaxPitch || (Pitch + f) < -MaxPitch)
		{
			return;
		}
	}
	Pitch += f;
}

void CCamara::MoveRoll(float f)
{
	if (MaxRoll != 0.0f)
	{
		if ((Roll + f) > MaxRoll || (Roll + f) < -MaxRoll)
		{
			return;
		}
	}
	Roll += f;
}

void CCamara::SetFov(float f)
{
	this->FOV = f;
	CreateProjection();
}

void CCamara::SetRatio(float f)
{
	this->AR = f;
	CreateProjection();
}

void CCamara::SetPlanes(float near, float far)
{
	this->NearPlane = near;
	this->FarPlane = far;
	CreateProjection();
}

void CCamara::Update(float DT)
{
	MATRIX4D XM, YM, ZM, TM;

	XM = RotationXRH(-Pitch);
	YM = RotationYRH(-Yaw);
	ZM = RotationZRH(-Roll);

	View = ZM * YM * XM;

	MATRIX4D TransposeM = Transpose(View);
	Look = TransformNormalRH(LookConstCameraSpace, TransposeM);
	Up = TransformNormalRH(UpConstCameraSpace, TransposeM);
	Right = TransformNormalRH(RIghtConstCameraSPace, TransposeM);

	Normalize(Look);
	Normalize(Up);
	Normalize(Right);

	VECTOR4D ActualVel = Right * Velocity.x + Up * Velocity.y + Look * Velocity.z;
	Velocity -= (Velocity * Friction);
	Eye = Eye + ActualVel;

	VECTOR4D TransformEye = Eye;
	TM = MatTranslation(TransformEye);
	View = TM * View;
	VP = View * Projection;
}

void CCamara::Reset()
{
	Eye = { 0.0f, 0.0f, 0.0f, 1.0f };
	Velocity = { 0.0f, 0.0f, 0.0f, 1.0f };
	FOV = (45.0f * (3.141592 / 180));
	NearPlane = 0.01f;
	FarPlane = 1000.0f;
	AR = 1.0f;
	Speed = 5000.0;
	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
	Friction = 0.1f;
	MaxRoll = 0.0f;
	MaxPitch = (89.0f *(3.141592 / 180));
	MaxYaw = 0.0f;
	LeftHanded = true;
}