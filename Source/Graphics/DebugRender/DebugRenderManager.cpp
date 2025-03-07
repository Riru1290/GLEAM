#include "../../Common/Primitive/Sphere/Sphere.h"
#include "../../Object/Component/Renderer/Renderers/Instancing/Primitive/SphereInstancingRenderer.h"
#include "../../Object/Component/Renderer/Renderers/Instancing/Primitive/BoxInstancingRenderer.h"
#include "../../Object/Component/Renderer/Renderers/Instancing/Primitive/CapsuleInstancingRenderer.h"
#include "../../Object/Component/Renderer/Renderers/Instancing/Primitive/CylinderInstancingRenderer.h"
#include "../RenderManager.h"
#include "DebugRenderManager.h"

DebugRenderManager::DebugRenderManager()
{
}

DebugRenderManager::~DebugRenderManager()
{
}

void DebugRenderManager::Terminate()
{
	renderers_.clear();
}

void DebugRenderManager::DrawSphere(Position3D center, float r, unsigned int color, float alpha, int fillFlag)
{
	if (renderers_.contains(PRIMITIVE_TYPE::SPHERE))
	{
		shared_ptr<SphereInstancingRenderer> sphereRenderer = 
			static_pointer_cast<SphereInstancingRenderer>(renderers_.at(PRIMITIVE_TYPE::SPHERE));

		sphereRenderer->Draw(center, r, color, alpha, fillFlag);
	}
	else
	{
		shared_ptr<PrimitiveInstancingRenderer> renderer = 
			RenderMng.CreateRenderer<SphereInstancingRenderer>();

		renderers_.emplace(PRIMITIVE_TYPE::SPHERE, renderer);

		shared_ptr<SphereInstancingRenderer> sphereRenderer =
			static_pointer_cast<SphereInstancingRenderer>(renderer);

		sphereRenderer->Draw(center, r, color, alpha, fillFlag);
	}
}

void DebugRenderManager::DrawBox(Position3D Pos1, Position3D Pos2, unsigned int color, float alpha, int fillFlag)
{
	if (renderers_.contains(PRIMITIVE_TYPE::BOX))
	{
		shared_ptr<BoxInstancingRenderer> boxRenderer =
			static_pointer_cast<BoxInstancingRenderer>(renderers_.at(PRIMITIVE_TYPE::BOX));

		boxRenderer->Draw(Pos1, Pos2, color, alpha, fillFlag);
	}
	else
	{
		shared_ptr<PrimitiveInstancingRenderer> renderer =
			RenderMng.CreateRenderer<BoxInstancingRenderer>();

		renderers_.emplace(PRIMITIVE_TYPE::BOX, renderer);

		shared_ptr<BoxInstancingRenderer> boxRenderer =
			static_pointer_cast<BoxInstancingRenderer>(renderer);

		boxRenderer->Draw(Pos1, Pos2, color, alpha, fillFlag);
	}
}

void DebugRenderManager::DrawCapsule(Position3D pos1, Position3D pos2, float r, unsigned int color, float alpha, int fillFlag)
{
	if (renderers_.contains(PRIMITIVE_TYPE::CAPSULE))
	{
		shared_ptr<CapsuleInstancingRenderer> capsuleRenderer =
			static_pointer_cast<CapsuleInstancingRenderer>(renderers_.at(PRIMITIVE_TYPE::CAPSULE));

		capsuleRenderer->Draw(pos1, pos2, r, color, alpha, fillFlag);
	}
	else
	{
		shared_ptr<CapsuleInstancingRenderer> renderer =
			RenderMng.CreateRenderer<CapsuleInstancingRenderer>();

		renderers_.emplace(PRIMITIVE_TYPE::CAPSULE, renderer);

		shared_ptr<CapsuleInstancingRenderer> capsuleRenderer =
			static_pointer_cast<CapsuleInstancingRenderer>(renderer);

		capsuleRenderer->Draw(pos1, pos2, r, color, alpha, fillFlag);
	}
}

void DebugRenderManager::DrawCylinder(Position3D pos, float height, float r, Vector3 angle, unsigned int color, float alpha, int fillFlag)
{
	if (renderers_.contains(PRIMITIVE_TYPE::CYLINDER))
	{
		shared_ptr<CylinderInstancingRenderer> cylinderRenderer =
			static_pointer_cast<CylinderInstancingRenderer>(renderers_.at(PRIMITIVE_TYPE::CYLINDER));

		cylinderRenderer->Draw(pos, height, r, angle, color, alpha, fillFlag);
	}
	else
	{
		shared_ptr<CylinderInstancingRenderer> renderer =
			RenderMng.CreateRenderer<CylinderInstancingRenderer>();



		renderers_.emplace(PRIMITIVE_TYPE::CYLINDER, renderer);

		shared_ptr<CylinderInstancingRenderer> cylinderRenderer =
			static_pointer_cast<CylinderInstancingRenderer>(renderer);

		cylinderRenderer->Draw(pos, height, r, angle, color, alpha, fillFlag);
	}
}
