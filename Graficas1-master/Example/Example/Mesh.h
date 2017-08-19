#ifndef UAD_MESH_GL_H
#define UAD_MESH_GL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Matrix4D.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <d3dx9math.h>

#include "PrimitiveBase.h"
#include "UtilsGL.h"

using namespace std;

struct MeshVertex {
	MeshVertex() : w(1.0f), nw(1.0f)
	{}
	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;

};

struct ContMesh
{
	MeshVertex *vertices;
	unsigned short	*indices;
	GLuint			VB;
	GLuint			IB;
	int vert, ind;
};

class CMesh : public PrimitiveBase {
public:
	float FTM[16];
	char separator;
	CMesh() : shaderID(0) {}
	void Create() {};
	void Create(char * t);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();


	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;
	ContMesh	*XMesh;
	vector <ContMesh*> Meshes;

	/*vector <MeshVertex> vertices;
	vector <unsigned short>	indices;
	GLuint			VB;
	GLuint			IB;*/

	MATRIX4D	transform;
};


#endif