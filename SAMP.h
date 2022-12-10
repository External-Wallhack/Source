#include <algorithm>
#include <iterator>

using namespace std;

int Ballas[]	= { 102, 103, 104 };
int Grove[]		= { 105, 106, 107 };
int Vagos[]		= { 108, 109, 110 };
int Aztec[]		= { 114, 115, 116 };
int Rifa[]		= { 173, 174, 175 };

D3DCOLOR getColorGangBySkinID(int SkinID) {
	if (find(begin(Ballas),			end(Ballas),	SkinID)		!= end(Ballas)) return D3DCOLOR_ARGB(255, 168, 83, 230);
	else if (find(begin(Grove),		end(Grove),		SkinID)		!= end(Grove)) return D3DCOLOR_ARGB(255, 51, 158, 56);
	else if (find(begin(Vagos),		end(Vagos),		SkinID)		!= end(Vagos)) return D3DCOLOR_ARGB(255, 207, 209, 59);
	else if (find(begin(Aztec),		end(Aztec),		SkinID)		!= end(Aztec)) return D3DCOLOR_ARGB(255, 59, 209, 202);
	else if (find(begin(Rifa),		end(Rifa),		SkinID)		!= end(Rifa)) return D3DCOLOR_ARGB(255, 59, 136, 209);
	else return D3DCOLOR_ARGB(255, 209, 82, 59);
}