#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void CMesh::Create(char* t) {
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS_Mesh.glsl");
	char *fsSourceP = file2string("FS_Mesh.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
	normalAttribLoc = glGetAttribLocation(shaderID, "Normal");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");

	diffuseAttribLoc = glGetUniformLocation(shaderID, "Diffuse");
	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
	matWorldUniformLoc = glGetUniformLocation(shaderID, "World");

	string currentline;
	ifstream myfile(t);
	if (myfile.is_open())
	{
		while (getline(myfile, currentline))
		{
			if (currentline == "xof 0303txt 0032")
			{
				cout << currentline << '\n';

			}
			
			int MatrizRelativo = currentline.find("FrameTransformMatrix relative");
			int findMesh = currentline.find("Mesh mesh");
			int findNormals = currentline.find("MeshNormals normals");
			int findTexture = currentline.find("MeshTextureCoords tc0");
			int findMaterialList = currentline.find("MeshMaterialList mtls");
			int findDiffuseMap = currentline.find("diffuseMap");
			/*if (MatrizRelativo != -1)
			{
			cout << currentline << '\n';
			for (int i = 0; i < 16; i++)
			{
			myfile >> FTM[i] >> separator;
			cout << FTM[i] << endl;
			}
			}*/
			if (findMesh != -1)
			{
				XMesh = new ContMesh;
				myfile >> XMesh->vert >> separator;
				cout << "vertices: "<< XMesh->vert << endl;
				XMesh->vertices = new MeshVertex[XMesh->vert];
				for (int i = 0; i < XMesh->vert; i++)
				{
					myfile >> XMesh->vertices[i].x >> separator >> XMesh->vertices[i].y >> separator >> XMesh->vertices[i].z >> separator >> separator;
					//vertices.push_back(V);
				}
				myfile >> XMesh->ind >> separator;
				cout <<"indices: "<< XMesh->ind << endl;
				XMesh->indices = new unsigned short[XMesh->ind*3];
				for (int i = 0; i < XMesh->ind * 3; i++)
				{
					myfile >> separator >> separator >> XMesh->indices[i] >> separator;
					i++;
					myfile >> XMesh->indices[i] >> separator;
					i++;
					myfile >> XMesh->indices[i] >> separator >> separator;
				}
			}
			if (findNormals != -1)
			{
				myfile >> XMesh->vert >> separator;
				cout <<"normals vertices: "<< XMesh->vert << endl;
				for (int i = 0; i < XMesh->vert; i++)
				{
					myfile >> XMesh->vertices[i].nx >> separator >> XMesh->vertices[i].ny >> separator >> XMesh->vertices[i].nz >> separator >> separator;
				}
			}
			if (findTexture != -1)
			{
				myfile >> XMesh->vert >> separator;
				cout << "texture vertices: " << XMesh->vert;
				for (int i = 0; i < XMesh->vert; i++)
				{
					myfile >> XMesh->vertices[i].s >> separator >> XMesh->vertices[i].t >> separator >> separator;
				}
			}
			if (findMaterialList != -1)
			{
				int ntriangles = 0;
				int nmaterial = 0;
				myfile >> XMesh->totalmaterial >> separator;
				totMat = XMesh->totalmaterial;
				myfile >> ntriangles >> separator;
				for (int i = 0; i < XMesh->totalmaterial; i++)
				{
					XMaterial = new Material;
					XMaterial->ind = 0;
					XMesh->materials.push_back(XMaterial);
				}

				for (int i = 0; i < ntriangles; i++)
				{
					myfile >> nmaterial >> separator;
					XMesh->materials[nmaterial]->indices.push_back(XMesh->indices[i * 3 + 0]);
					XMesh->materials[nmaterial]->indices.push_back(XMesh->indices[i * 3 + 1]);
					XMesh->materials[nmaterial]->indices.push_back(XMesh->indices[i * 3 + 2]);
					XMesh->materials[nmaterial]->ind++;
				}
				contDiffuse = 0;
			}
			if (findDiffuseMap != -1)
			{
				myfile >> XMesh->materials[contDiffuse]->diffusepath;
				XMesh->materials[contDiffuse]->diffusepath = XMesh->materials[contDiffuse]->diffusepath.substr(1, XMesh->materials[contDiffuse]->diffusepath.size() - 3);
				cout << XMesh->materials[contDiffuse]->diffusepath;
				contDiffuse++;
			}
			if (contDiffuse == totMat)
			{
				contDiffuse = 0;
				while (contDiffuse < XMesh->totalmaterial)
				{
					int x = 0, y = 0, channels = 0;
					string path = "Textures/";
					path += XMesh->materials[contDiffuse]->diffusepath;
					unsigned char *buffer = stbi_load(path.c_str(), &x, &y, &channels, 0);

					glGenTextures(1, &XMesh->materials[contDiffuse]->diffuse_textID);
					glBindTexture(GL_TEXTURE_2D, XMesh->materials[contDiffuse]->diffuse_textID);

					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(buffer));
					glGenerateMipmap(GL_TEXTURE_2D);
					++contDiffuse;
				}
				contDiffuse = 0;
				Meshes.push_back(XMesh);
			}
			//cout << currentline << '\n';
		}
		myfile.close();
	}

	else
	{
		cout << "invalid file type: exiting." << endl;
	}
	//// +Y SIDE
	//vertices[0] = { -1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	//vertices[1] = { 1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	//vertices[2] = { -1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	//vertices[3] = { 1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f };

	//// -Y SIDE
	//vertices[4] = { -1.0f,  -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f };
	//vertices[5] = { 1.0f,  -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	//vertices[6] = { -1.0f,  -1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	//vertices[7] = { 1.0f,  -1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f };

	//// +X SIDE
	//vertices[8] = { 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	//vertices[9] = { 1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	//vertices[10] = { 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	//vertices[11] = { 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f };

	//// -X SIDE
	//vertices[12] = { -1.0f,  1.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f };
	//vertices[13] = { -1.0f,  1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	//vertices[14] = { -1.0f, -1.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	//vertices[15] = { -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f };

	//// +Z SIDE
	//vertices[16] = { -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f };
	//vertices[17] = { 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f };
	//vertices[18] = { -1.0f, -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f };
	//vertices[19] = { 1.0f, -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f };

	//// -Z SIDE
	//vertices[20] = { -1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  1.0f, 0.0f };
	//vertices[21] = { 1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f };
	//vertices[22] = { -1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  1.0f, 1.0f };
	//vertices[23] = { 1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f };

	//// +X
	//indices[0] = 8;
	//indices[1] = 9;
	//indices[2] = 10;
	//indices[3] = 9;
	//indices[4] = 11;
	//indices[5] = 10;

	//// -X
	//indices[6] = 14;
	//indices[7] = 13;
	//indices[8] = 12;
	//indices[9] = 14;
	//indices[10] = 15;
	//indices[11] = 13;

	//// +Y
	//indices[12] = 1;
	//indices[13] = 2;
	//indices[14] = 0;
	//indices[15] = 3;
	//indices[16] = 2;
	//indices[17] = 1;

	//// -Y
	//indices[18] = 4;
	//indices[19] = 6;
	//indices[20] = 5;
	//indices[21] = 5;
	//indices[22] = 6;
	//indices[23] = 7;

	//// +Z
	//indices[24] = 17;
	//indices[25] = 18;
	//indices[26] = 16;
	//indices[27] = 19;
	//indices[28] = 18;
	//indices[29] = 17;

	//// -Z
	//indices[30] = 20;
	//indices[31] = 22;
	//indices[32] = 21;
	//indices[33] = 21;
	//indices[34] = 22;
	//indices[35] = 23;

	for (int i = 0; i < Meshes.size(); i++)
	{
		glGenBuffers(1, &Meshes[i]->VB);
		glBindBuffer(GL_ARRAY_BUFFER, Meshes[i]->VB);
		glBufferData(GL_ARRAY_BUFFER, Meshes[i]->vert * sizeof(MeshVertex), Meshes[i]->vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &Meshes[i]->IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Meshes[i]->IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (Meshes[i]->ind * 3) * sizeof(unsigned short), Meshes[i]->indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (int j = 0; j < Meshes[i]->totalmaterial; j++)
		{
			glGenBuffers(1, &Meshes[i]->materials[j]->IB);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Meshes[i]->materials[j]->IB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (Meshes[i]->materials[j]->ind * 3) * sizeof(unsigned short), &Meshes[i]->materials[j]->indices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	transform = Identity();
}

void CMesh::Transform(float *t) {
	for (int i = 0; i < 16; i++)
	{
		transform.v[i] = t[i];
	}
}

void CMesh::Draw(float *t, float *vp) {
	glUseProgram(shaderID);

	if (t)
		for (int i = 0; i < 16; i++)
		{
			transform.v[i] = t[i];
		}

	MATRIX4D VP;
	for (int i = 0; i < 16; i++)
	{
		VP.v[i] = vp[i];
	}
	MATRIX4D WVP = transform*VP;

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
	
	for (int i = 0; i < Meshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, Meshes[i]->VB);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Meshes[i]->IB);

		glEnableVertexAttribArray(vertexAttribLoc);
		//glEnableVertexAttribArray(normalAttribLoc);

		if (uvAttribLoc != -1)
			glEnableVertexAttribArray(uvAttribLoc);

		if (normalAttribLoc != -1)
		{
			glEnableVertexAttribArray(normalAttribLoc);
		}

		glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(0));
		glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(16));

		if (uvAttribLoc != -1)
			glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(32));
		
		for (int j = 0; j < Meshes[i]->materials.size(); j++)
		{
			if (diffuseAttribLoc != -1)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Meshes[i]->materials[j]->diffuse_textID);
				glUniform1i(diffuseAttribLoc, 0);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Meshes[i]->materials[j]->IB);
			//glDrawElements(GL_TRIANGLES, Meshes[i]->ind * 3, GL_UNSIGNED_SHORT, 0);
			glDrawElements(GL_TRIANGLES, Meshes[i]->materials[j]->ind * 3, GL_UNSIGNED_SHORT, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vertexAttribLoc);
		/*glDisableVertexAttribArray(normalAttribLoc);

		if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
		}*/
		if (normalAttribLoc != -1)
		{
			glDisableVertexAttribArray(normalAttribLoc);
		}
	}
	glUseProgram(0);
}

void CMesh::Destroy() {
	glDeleteProgram(shaderID);
}