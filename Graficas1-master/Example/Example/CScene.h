#pragma once
#include "Matrix4D.h"
#include "CCamara.h"
#include <vector>

struct Light
{
	VECTOR4D Position;
	VECTOR4D Color;
	int Type;
	int Enable;
};

struct CScene
{
public:
	CScene() : ActiveCam(0), ActiveLight(1), ActiveLightCamera(0) {}

	void CreateLight(VECTOR4D Pos, VECTOR4D Clr, bool active);
	void DeleteLight(unsigned int ind);
	void SetLightPos(unsigned int ind, VECTOR4D Pos);

	void CreateCamera(CCamara * MyPCamera);
	void DeleteCamera(unsigned int ind);

	void CreateLightCamera(CCamara * LightCam);
	void DeleteLightCamera(unsigned int ind);

	std::vector<Light>LightContainer;
	std::vector<CCamara*>CameraContainer;
	std::vector<CCamara*>CameraLightCont;

	int ActiveCam;
	int ActiveLight;
	int ActiveLightCamera;

	VECTOR4D AmbientClr;
};