#include "../../../Resource/ResourceManager.h"
#include "../../../Object/Component/Common/Transform.h"
#include "NumberUI.h"

NumberUI::NumberUI(const int& num):
num_(num)
{
	SetTexture(ResourceMng.GetTexture(RES_ID::IMAGE_NUMBER), 10, 1);
}

void NumberUI::UpdateUI()
{
	if (spriteRenderer_.has_value() && num_.has_value()) {
		
		Transform base = transform_->get();

		const int& num = num_->get();

		int check = num;

		if (check == 0)
		{
			spriteRenderer_->get().RenderSprite(base, 0);
			return;
		}
		vector<int> disNo;
		while (check > 0) {
			int no = check;
			check /= 10;

			spriteRenderer_->get().RenderSprite(base, no - check * 10);

			base.pos.x -= offsetX_;
		}
	}
}

void NumberUI::InitAfterSetTexture()
{
	if (spriteRenderer_.has_value()) {
		offsetX_ = spriteRenderer_->get().GetTextureSize().x;
	}
}
