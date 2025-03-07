#include "Transform.h"

Transform::Transform() : Component(),
	pos({ 0.0f,0.0f,0.0f }),
	qua({ 0.0f,0.0f,0.0f }),
	scale({ 1.0f,1.0f,1.0f }),
	localPos({ 0.0f,0.0f,0.0f }),
	localQua({ 0.0f,0.0f,0.0f }),
	localScale({ 1.0f,1.0f,1.0f }),
	prePos({ 0.0f,0.0f,0.0f }),
	preQua({ 0.0f,0.0f,0.0f }),
	preScale({ 1.0f,1.0f,1.0f })
{
}

Transform::Transform(Position3D initPos, Quaternion initQua, Scale3D initScale) : Component(),
pos(initPos),
qua(initQua),
scale(initScale),
localPos({ 0.0f,0.0f,0.0f }),
localQua({ 0.0f,0.0f,0.0f }),
localScale({ 1.0f,1.0f,1.0f }),
prePos({ 0.0f,0.0f,0.0f }),
preQua({ 0.0f,0.0f,0.0f }),
preScale({ 1.0f,1.0f,1.0f })
{
}

void Transform::PreUpdateComponent()
{
	prePos = pos;
	preScale = scale;
	preQua = qua;
}
