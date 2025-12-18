#include "O_Obj.h"

void O_Obj::Updata()
{
	move += 0.02f;
	world = XMMatrixTranslation(0.0f, std::sinf(move) * 1.5f, 0.0f);
	color = XMFLOAT4(0.1f, 1.0f, 1.0f, 1.0f);
}