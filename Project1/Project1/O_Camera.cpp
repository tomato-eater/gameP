#include "O_Camera.h"

float eyeMoveSpeed = 0.06f;
float destTargetToView = -5.0f;

void O_Camera::Instalize(int width, int height)
{
	position = XMFLOAT3(0.0f, 0.0f, destTargetToView);
	target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	projection = XMMatrixPerspectiveFovLH
	(
		XM_PIDIV4,
		width / height,
		0.1f, 100.0f
	);
}

void O_Camera::Updata()
{
	float angle = 0.0f;
	angle += eyeMoveSpeed;
	position.x = destTargetToView * std::sinf(angle);
	position.z = destTargetToView * std::cosf(angle);

	view = XMMatrixLookAtLH
	(
		XMLoadFloat3(&position),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);
}