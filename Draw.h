#include "main.h"

INT OVERLAY_WIDTH = 800;
INT OVERLAY_HEIGHT = 600;

HWND OVERLAY_WINDOW;

const MARGINS margin = { 0,0,OVERLAY_WIDTH,OVERLAY_HEIGHT };

LPDIRECT3DDEVICE9 pDevice;
LPD3DXFONT pFont;
LPDIRECT3DVERTEXBUFFER9 g_pVB;    // Buffer to hold vertices
LPDIRECT3DINDEXBUFFER9  g_pIB;    // Buffer to hold indices

int FontNr;
LPD3DXSPRITE sSprite;

enum circle_type { full, half, quarter };
enum text_alignment { lefted, centered, righted };

#define MAX_FONTS 6
#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)
#define BLACK(alpha)         D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define RED(alpha)           D3DCOLOR_ARGB(alpha, 255, 0, 0)
#define GREEN(alpha)         D3DCOLOR_ARGB(alpha, 0, 128, 0)
#define LAWNGREEN(alpha)     D3DCOLOR_ARGB(alpha, 124, 252, 0)
#define BLUE(alpha)          D3DCOLOR_ARGB(alpha, 0, 200, 255)
#define DEEPSKYBLUE(alpha)   D3DCOLOR_ARGB(alpha, 0, 191, 255)
#define SKYBLUE(alpha)       D3DCOLOR_ARGB(alpha, 0, 122, 204)
#define YELLOW(alpha)        D3DCOLOR_ARGB(alpha, 255, 255, 0)
#define ORANGE(alpha)        D3DCOLOR_ARGB(alpha, 255, 165, 0)
#define DARKORANGE(alpha)    D3DCOLOR_ARGB(alpha, 255, 140, 0)
#define PURPLE(alpha)        D3DCOLOR_ARGB(alpha, 125, 0, 255)
#define CYAN(alpha)          D3DCOLOR_ARGB(alpha, 0, 255, 255)
#define PINK(alpha)          D3DCOLOR_ARGB(alpha, 255, 20, 147)
#define GRAY(alpha)          D3DCOLOR_ARGB(alpha, 128, 128, 128)
#define DARKGRAY(alpha)      D3DCOLOR_ARGB(alpha, 73, 73, 73)
#define DARKERGRAY(alpha)    D3DCOLOR_ARGB(alpha, 31, 31, 31)

struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

struct sScreen
{
	float Width;
	float Height;
	float x_center;
	float y_center;
} Screen;

void DrawBoxFilled(float x, float y, float w, float h, DWORD color)
{
	vertex V[4];

	V[0].color = V[1].color = V[2].color = V[3].color = color;

	V[0].z = V[1].z = V[2].z = V[3].z = 0;
	V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

	V[0].x = x;
	V[0].y = y;
	V[1].x = x + w;
	V[1].y = y;
	V[2].x = x + w;
	V[2].y = y + h;
	V[3].x = x;
	V[3].y = y + h;

	unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };

	pDevice->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);
	pDevice->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, sizeof(V), (void**)&pVertices, 0);
	memcpy(pVertices, V, sizeof(V));
	g_pVB->Unlock();

	VOID* pIndex;
	g_pIB->Lock(0, sizeof(indexes), (void**)&pIndex, 0);
	memcpy(pIndex, indexes, sizeof(indexes));
	g_pIB->Unlock();

	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->SetIndices(g_pIB);


	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);


	g_pVB->Release();
	g_pIB->Release();
}

void Reset()
{
	D3DVIEWPORT9 screen;
	pDevice->GetViewport(&screen);

	Screen.Width = screen.Width;
	Screen.Height = screen.Height;
	Screen.x_center = Screen.Width / 2;
	Screen.y_center = Screen.Height / 2;
}

void DrawLine(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color)
{
	ID3DXLine* m_Line;

	D3DXCreateLine(pDevice, &m_Line);
	D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
	m_Line->SetWidth(width);
	if (antialias)
		m_Line->SetAntialias(antialias);
	m_Line->Begin();
	m_Line->Draw(line, 2, color);
	m_Line->End();
	m_Line->Release();
}

void DrawCircle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color)
{
	std::vector<vertex> circle(resolution + 2);
	float angle = rotate * D3DX_PI / 180;
	float pi;

	if (type == full) pi = D3DX_PI;        // Full circle
	if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
	if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

	for (int i = 0; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - radius * cos(i * (2 * pi / resolution)));
		circle[i].y = (float)(y - radius * sin(i * (2 * pi / resolution)));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
		circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
	}

	pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
	g_pVB->Unlock();


	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(NULL);
	if (smoothing)
	{
		pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	}
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
	if (g_pVB != NULL) g_pVB->Release();
}

void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, string str) {
	RECT fontPos = { x, y, x + 120, y + 16 };
	g_pFont->DrawTextA(nullptr, str.c_str(), -1, &fontPos, DT_NOCLIP, color);
}

void DrawCircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color)
{
	std::vector<vertex> circle(resolution + 2);
	float angle = rotate * D3DX_PI / 180;
	float pi;

	if (type == full) pi = D3DX_PI;        // Full circle
	if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
	if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
		circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
	}

	pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
	g_pVB->Unlock();

	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB != NULL) g_pVB->Release();
}

void DrawBox(float x, float y, float w, float h, float linewidth, DWORD color)
{
	if (linewidth == 0 || linewidth == 1)
	{
		DrawBoxFilled(x, y, w, 1, color);             // Top
		DrawBoxFilled(x, y + h - 1, w, 1, color);         // Bottom
		DrawBoxFilled(x, y + 1, 1, h - 2 * 1, color);       // Left
		DrawBoxFilled(x + w - 1, y + 1, 1, h - 2 * 1, color);   // Right
	}
	else
	{
		DrawBoxFilled(x, y, w, linewidth, color);                                     // Top
		DrawBoxFilled(x, y + h - linewidth, w, linewidth, color);                         // Bottom
		DrawBoxFilled(x, y + linewidth, linewidth, h - 2 * linewidth, color);               // Left
		DrawBoxFilled(x + w - linewidth, y + linewidth, linewidth, h - 2 * linewidth, color);   // Right
	}
}

void DrawBoxBordered(float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border)
{
	DrawBoxFilled(x, y, w, h, color);
	DrawBox(x - border_width, y - border_width, w + 2 * border_width, h + border_width, border_width, color_border);
}

void DrawBoxRounded(float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor)
{
	DrawBoxFilled(x + radius, y + radius, w - 2 * radius - 1, h - 2 * radius - 1, color);   // Center rect.
	DrawBoxFilled(x + radius, y + 1, w - 2 * radius - 1, radius - 1, color);            // Top rect.
	DrawBoxFilled(x + radius, y + h - radius - 1, w - 2 * radius - 1, radius, color);     // Bottom rect.
	DrawBoxFilled(x + 1, y + radius, radius - 1, h - 2 * radius - 1, color);            // Left rect.
	DrawBoxFilled(x + w - radius - 1, y + radius, radius, h - 2 * radius - 1, color);     // Right rect.

	// Smoothing method
	if (smoothing)
	{
		DrawCircleFilled(x + radius, y + radius, radius - 1, 0, quarter, 16, color);             // Top-left corner
		DrawCircleFilled(x + w - radius - 1, y + radius, radius - 1, 90, quarter, 16, color);        // Top-right corner
		DrawCircleFilled(x + w - radius - 1, y + h - radius - 1, radius - 1, 180, quarter, 16, color);   // Bottom-right corner
		DrawCircleFilled(x + radius, y + h - radius - 1, radius - 1, 270, quarter, 16, color);       // Bottom-left corner

		DrawCircle(x + radius + 1, y + radius + 1, radius, 0, quarter, true, 16, bcolor);          // Top-left corner
		DrawCircle(x + w - radius - 2, y + radius + 1, radius, 90, quarter, true, 16, bcolor);       // Top-right corner
		DrawCircle(x + w - radius - 2, y + h - radius - 2, radius, 180, quarter, true, 16, bcolor);    // Bottom-right corner
		DrawCircle(x + radius + 1, y + h - radius - 2, radius, 270, quarter, true, 16, bcolor);      // Bottom-left corner

		DrawLine(x + radius, y + 1, x + w - radius - 1, y + 1, 1, false, bcolor);       // Top line
		DrawLine(x + radius, y + h - 2, x + w - radius - 1, y + h - 2, 1, false, bcolor);   // Bottom line
		DrawLine(x + 1, y + radius, x + 1, y + h - radius - 1, 1, false, bcolor);       // Left line
		DrawLine(x + w - 2, y + radius, x + w - 2, y + h - radius - 1, 1, false, bcolor);   // Right line
	}
	else
	{
		DrawCircleFilled(x + radius, y + radius, radius, 0, quarter, 16, color);             // Top-left corner
		DrawCircleFilled(x + w - radius - 1, y + radius, radius, 90, quarter, 16, color);        // Top-right corner
		DrawCircleFilled(x + w - radius - 1, y + h - radius - 1, radius, 180, quarter, 16, color);   // Bottom-right corner
		DrawCircleFilled(x + radius, y + h - radius - 1, radius, 270, quarter, 16, color);       // Bottom-left corner
	}
}

void DrawSprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation)
{
	float screen_pos_x = x;
	float screen_pos_y = y;

	// Texture being used is 64x64:
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution / 2, resolution / 2);

	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(screen_pos_x, screen_pos_y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(scale, scale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);

	//pDevice->SetRenderState(D3DRS_ZENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetPixelShader(NULL);
	sSprite->Begin(NULL);
	sSprite->SetTransform(&mat); // Tell the sprite about the matrix
	sSprite->Draw(tex, NULL, NULL, NULL, 0xFFFFFFFF);
	sSprite->End();
}

void initD3D(HWND hWnd) {
	BOOL bCompositionEnabled = false;
	DWORD msqAAQuality = 0;

	DwmIsCompositionEnabled(&bCompositionEnabled);

	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferWidth = OVERLAY_WIDTH;
	d3dpp.BackBufferHeight = OVERLAY_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice);
	D3DXCreateFont(pDevice, 16, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);
}

void DrawScene();

void RenderESP(bool statusCheat) {
	pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
	pDevice->BeginScene();

	if (!statusCheat) {
		DrawString(15, 15, D3DCOLOR_ARGB(255, 255, 57, 43), pFont, "BLACKMODE");
	}
	else {
		DrawString(15, 15, D3DCOLOR_ARGB(255, 43, 255, 121), pFont, "BLACKMODE");
		DrawScene();
	};

	pDevice->EndScene();
	pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}