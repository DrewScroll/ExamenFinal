#include "Application.h"
#include <iostream>

#define LIGHT_COUNT	1

using namespace std;

#define M_PI 3.141592654

void TestApp::InitVars() {
	DtTimer.Init();
	DtTimer.Update();
	srand((unsigned int)DtTimer.GetDTSecs());
	//Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//Orientation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//Scaling		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	Position = { 0,0,0,0 };
	Orientation = { 0,0,0,0 };
	Scaling = { 1,1,1,0 };
	Camara.Init(VECTOR4D{ 0.0f,1.0f,10.0f,0.0f }, 45 * (M_PI / 180), 1280.0f / 720.0f, 1.0f, 8000.0f);
	Camara.Speed = 10.0f;
	Camara.Eye = VECTOR4D{ 0.0f, 9.75f, -31.0f, 0.0f };
	Camara.Pitch = 0.14f;
	Camara.Roll = 0.0f;
	Camara.Yaw = 0.020f;
	Camara.Update(0.0f);
	
	LuzCamara.Init(VECTOR4D{ 0.0f,1.0f,10.0f,0.0f }, 60.0f*(M_PI / 180), 1.0f, 0.1f, 8000.0f, false);
	LuzCamara.Speed = 10.0f;
	LuzCamara.Eye = VECTOR4D{ 0.0f, 25.0f, -40.0f, 0.0f };
	LuzCamara.Pitch = 0.14f;
	LuzCamara.Roll = 0.0f;
	LuzCamara.Yaw = 0.020f;
	LuzCamara.Update(0.0f);

	CamaraActiva = &Camara;
	Scene.CreateCamera(CamaraActiva);
	Scene.CreateCamera(CamaraActiva);
	Scene.AmbientClr = { .5f, .5f, .5f, 1.0f };
	Scene.CreateLightCamera(&LuzCamara);
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Scene.CreateLight({ 5.f, 0.0f, 0.0f, 1.0f }, { 0.0f, 255.0f, 255.0f, 1.0f }, true);
	}
	FirstFrame = true;
}

void TestApp::CreateAssets() {
	PrimitiveMgr.SetVP(&VP);
	//int indexCube = PrimitiveMgr.CreateCube();
	//Cubes[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCube), &VP);
	//Cubes[1].CreateInstance(PrimitiveMgr.GetPrimitive(indexCube), &VP);

	//int indexTri = PrimitiveMgr.CreateCube();
	int indexMesh = PrimitiveMgr.CreateMesh("NuBatman.X");
	//Triangle[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexTri), &VP);

	Mesh[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexMesh), &VP);

	/*D3DXMATRIX View;
	D3DXVECTOR3 Pos		= D3DXVECTOR3(0.0f,1.0f,5.0f);
	D3DXVECTOR3 Up		= D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVECTOR3 LookAt	= D3DXVECTOR3(0.0001f, 0.0001f, 0.0001f) - Pos;
	D3DXMatrixLookAtRH(&View,&Pos,&LookAt,&Up);
	D3DXMATRIX Proj;*/

	VECTOR4D Pos = { 0,1,-5,0 };
	VECTOR4D Up = { 0,1,0,0 };
	VECTOR4D LookAt = { 0.0001,0.0001,0.0001,0 };
	View = LookAtRH(Pos, LookAt, Up);
	Projection = PerspectiveFOVRH((M_PI / 4), (1280.0f / 720.0f), 0.1, 8000);

	//D3DXMatrixPerspectiveFovRH(&Proj,D3DXToRadian(45.0f),1280.0f/720.0f,0.1f,1000.0f);
	//	D3DXMatrixOrthoRH(&Proj, 1280.0f / 720.0f, 1.0f , 0.1, 100.0f);
	VP = View*Projection;
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();
	Dtsecs = DtTimer.GetDTSecs();
	OnInput();
	CamaraActiva->Update(Dtsecs);
	VP = CamaraActiva->VP;

	/*Triangle[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	Triangle[0].RotateXAbsolute(Orientation.x);
	Triangle[0].RotateYAbsolute(Orientation.y);
	Triangle[0].RotateZAbsolute(Orientation.z);
	Triangle[0].ScaleAbsolute(Scaling.x);
	Triangle[0].Update();*/
	
	Mesh[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	Mesh[0].RotateXAbsolute(Orientation.x);
	Mesh[0].RotateYAbsolute(Orientation.y);
	Mesh[0].RotateZAbsolute(Orientation.z);
	Mesh[0].ScaleAbsolute(Scaling.x);
	Mesh[0].Update();
	
	Cubes[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	Cubes[0].RotateXAbsolute(Orientation.x);
	Cubes[0].RotateYAbsolute(Orientation.y);
	Cubes[0].RotateZAbsolute(Orientation.z);
	Cubes[0].ScaleAbsolute(Scaling.x);
	Cubes[0].TranslateRelative(-Scene.LightContainer[0].Position.x, -Scene.LightContainer[0].Position.y, -Scene.LightContainer[0].Position.z);
	Cubes[0].Update();
	/*
	Cubes[1].TranslateAbsolute(-Position.x,-Position.y, Position.z);
	Cubes[1].RotateXAbsolute(-Orientation.x);
	Cubes[1].RotateYAbsolute(-Orientation.y);
	Cubes[1].RotateZAbsolute(-Orientation.z);
	Cubes[1].ScaleAbsolute(Scaling.x);
	Cubes[1].Update();
	*/
	float speed = 0.5f;
	static float freq = 0.0f;
	freq += Dtsecs*speed;
	static float freq2 = M_PI / 2.0f;
	freq2 += Dtsecs*speed;
	PrimitiveInst *Sel = &Mesh[5];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	//Triangle[0].Draw();
	Mesh[0].Draw();
	pFramework->pVideoDriver->SwapBuffers();
	FirstFrame = false;
}

void TestApp::OnInput() {
	if (FirstFrame)
	{
		return;
	}
	bool cambio = false;
	const float vel = 10.0f;
	if (IManager.PressedKey(SDLK_UP)) {
		Scene.LightContainer[0].Position.y -= 10.0f * DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Scene.LightContainer[0].Position.y += 10.0f * DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Scene.LightContainer[0].Position.x -= 10.0f * DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Scene.LightContainer[0].Position.x += 10.0f * DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		Scene.LightContainer[0].Position.z += 10.0f * DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		Scene.LightContainer[0].Position.z -= 10.0f * DtTimer.GetDTSecs();		
	}

	if (IManager.PressedKey(SDLK_KP_PLUS)) {
		Scaling.x += 1.0f*DtTimer.GetDTSecs();
		Scaling.y += 1.0f*DtTimer.GetDTSecs();
		Scaling.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_MINUS)) {
		Scaling.x -= 1.0f*DtTimer.GetDTSecs();
		Scaling.y -= 1.0f*DtTimer.GetDTSecs();
		Scaling.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		Orientation.x -= 60.0f*vel*Dtsecs;
		cambio = true;
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		Orientation.x += 60.0f*vel*Dtsecs;
		cambio = true;
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		Orientation.y -= 60.0f*vel*Dtsecs;
		cambio = true;
	}

	if (IManager.PressedKey(SDLK_KP3)) {
		Orientation.y += 60.0f*vel*Dtsecs;
		cambio = true;
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		Orientation.z -= 60.0f*vel*Dtsecs;
		cambio = true;
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*vel*Dtsecs;
		cambio = true;
	}
	if (IManager.PressedKey(SDLK_w))
	{
		CamaraActiva->MoveFront(Dtsecs);
	}
	if (IManager.PressedKey(SDLK_s))
	{
		CamaraActiva->MoveBack(Dtsecs);
	}
	if (IManager.PressedKey(SDLK_a))
	{
		CamaraActiva->MoveLeft(Dtsecs);
	}
	if (IManager.PressedKey(SDLK_d))
	{
		CamaraActiva->MoveRight(Dtsecs);
	}
	float yaw = 0.005f *static_cast<float>(IManager.Deltax);
	CamaraActiva->MoveYaw(yaw);
	float pitch = 0.005f*static_cast<float>(IManager.Deltay);
	CamaraActiva->MovePitch(pitch);
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}