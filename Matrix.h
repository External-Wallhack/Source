#pragma once

#include <iostream>
#include <d3dx9math.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>
#include "Memory.h"

using namespace std;

struct Resolution {
	DWORD Width, Heigth, Depth;
};

class Matrix {
public:
	D3DVECTOR CalcScreenCoors(Resolution Resolution, D3DXMATRIX m, D3DVECTOR vecWorld)
	{
		float xPos = (vecWorld.z * m._31) + (vecWorld.y * m._21) + (vecWorld.x * m._11) + m._41;
		float yPos = (vecWorld.z * m._32) + (vecWorld.y * m._22) + (vecWorld.x * m._12) + m._42;
		float zPos = (vecWorld.z * m._33) + (vecWorld.y * m._23) + (vecWorld.x * m._13) + m._43;

		double    fRecip = (double)1.0 / zPos;
		xPos *= (float)(fRecip * Resolution.Width);
		yPos *= (float)(fRecip * Resolution.Heigth);
		
		return D3DVECTOR{ xPos,yPos,zPos };
	}

	double Get3DDistance(D3DVECTOR myCoords, D3DVECTOR enemyCoords) {
		return sqrt(
			pow(double(enemyCoords.x - myCoords.x), 2.0) +
			pow(double(enemyCoords.y - myCoords.y), 2.0) +
			pow(double(enemyCoords.z - myCoords.z), 2.0));
	}
protected:
	Memory mem;
};