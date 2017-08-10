#include "PrimitiveInstance.h"

void PrimitiveInst::TranslateAbsolute(float x, float y, float z){
	//D3DXMatrixTranslation(&Position, x, y, z);
	Position = TranslationLH(x, y, z);
}

void PrimitiveInst::RotateXAbsolute(float ang) {
	//D3DXMatrixRotationX(&RotationX,D3DXToRadian(ang));
	RotationX = RotationXLH(ang*(M_PI/180));
}

void PrimitiveInst::RotateYAbsolute(float ang) {
	//D3DXMatrixRotationY(&RotationY, D3DXToRadian(ang));
	RotationY = RotationYLH(ang*(M_PI / 180));
}

void PrimitiveInst::RotateZAbsolute(float ang) {
	//D3DXMatrixRotationZ(&RotationZ, D3DXToRadian(ang));
	RotationZ = RotationZLH(ang*(M_PI / 180));
}

void PrimitiveInst::ScaleAbsolute(float sc) {
	//D3DXMatrixScaling(&Scale,sc,sc,sc);
	Scale = Scaling(sc, sc, sc);
}

void PrimitiveInst::TranslateRelative(float x, float y, float z) {
	/*D3DXMATRIX tmp;
	D3DXMatrixTranslation(&tmp, x, y, z);*/
	MATRIX4D tmp;
	tmp = TranslationLH(x, y, z);
	Position = Position * tmp;
}

void PrimitiveInst::RotateXRelative(float ang) {
	/*D3DXMATRIX tmp;
	D3DXMatrixRotationX(&tmp, D3DXToRadian(ang));*/
	MATRIX4D tmp;
	tmp = RotationXLH(ang*(M_PI / 180));
	RotationX = RotationX * tmp;
}

void PrimitiveInst::RotateYRelative(float ang) {
	/*D3DXMATRIX tmp;
	D3DXMatrixRotationY(&tmp, D3DXToRadian(ang));*/
	MATRIX4D tmp;
	tmp = RotationYLH(ang*(M_PI / 180));
	RotationY = RotationY * tmp;
}

void PrimitiveInst::RotateZRelative(float ang) {
	/*D3DXMATRIX tmp;
	D3DXMatrixRotationZ(&tmp, D3DXToRadian(ang));*/
	MATRIX4D tmp;
	tmp = RotationZLH(ang*(M_PI / 180));
	RotationZ = RotationZ * tmp;
}

void PrimitiveInst::ScaleRelative(float sc) {
	/*D3DXMATRIX tmp;
	D3DXMatrixScaling(&tmp, sc, sc, sc);*/
	MATRIX4D tmp;
	tmp = Scaling(sc, sc, sc);
	Scale = Scale * tmp;
}

void PrimitiveInst::Update() {
	Final = Scale*RotationX*RotationY*RotationZ*Position;
}

void PrimitiveInst::Draw(){
	pBase->Draw(&Final.m[0][0],&(*pViewProj).m[0][0]);
}
