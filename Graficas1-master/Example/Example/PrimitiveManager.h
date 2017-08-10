#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include <vector>
#include <d3dx9math.h>
#include "PrimitiveBase.h"
#include "Matrix4D.h"

class PrimitiveManager {
public:
	void SetVP(MATRIX4D *vp) {
		pVP = vp;
	}
	/*void SetVP(D3DXMATRIX *vp) {
		pVP = vp;
	}*/
	int  CreateTriangle();
	int	 CreateCube();
	int CreateMesh(char* t);

	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);

	std::vector<PrimitiveBase*> primitives;

	MATRIX4D *pVP;
	//D3DXMATRIX *pVP;
};

#endif