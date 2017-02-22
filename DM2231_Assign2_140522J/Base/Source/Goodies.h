#pragma once

#include "Mesh.h"

class CGoodies
{
public:
	CGoodies(void);
	~CGoodies(void);
protected:
	//position x of the goodies
	int pos_x;
	//position y of the goodies
	int pos_y;

public:
	//stores the texture for the goodies
	Mesh* theMesh;

	//get the pos_x of the goodies
	int GetPos_x(void);
	//get the pos_y of the goodies
	int GetPos_y(void);
	//Set the pos_x and pos_y of the goodies
	void SetPos(const int pos_x, const int pos_y);

	//get the mesh
	Mesh* GetMesh(void);
	//set the mesh
	void SetMesh(Mesh* theMesh);
	//set the texture ID
	void SetTextureID(const int TextureID);
};