#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>

#include <d3dx9math.h>
#include <Timer.h>
#include "CCamara.h"

class TestApp : public AppBase {
public:
	TestApp() : AppBase() {}
	void InitVars();
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	PrimitiveManager PrimitiveMgr;
	PrimitiveInst	Cubes[10];
	PrimitiveInst	Triangle[10];
	PrimitiveInst Mesh[10];

	CCamara Camara;
	CCamara *CamaraActiva;
	CCamara LuzCamara;
	
	//D3DXVECTOR3		Position;
	//D3DXVECTOR3		Orientation;
	//D3DXVECTOR3		Scaling;
	//D3DXMATRIX		WorldTransform;

	VECTOR4D Position;
	VECTOR4D Orientation;
	VECTOR4D Scaling;
	MATRIX4D WorldTransform;

	//D3DXMATRIX		View;
	//D3DXMATRIX		Projection;
	//D3DXMATRIX		VP;

	MATRIX4D View;
	MATRIX4D Projection;
	MATRIX4D VP;

	bool	FirstFrame;
	Timer	DtTimer;
	float Dtsecs;
	int CamSelection;
};