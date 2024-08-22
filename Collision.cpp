#include "Collision.h"

void Collision::resolve()
{
}

bool Collision::isColliding() const
{
	return false;
}

DirectX::XMFLOAT3 Collision::getContactPoint() const
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Collision::getContactNormal() const
{
	return DirectX::XMFLOAT3();
}
