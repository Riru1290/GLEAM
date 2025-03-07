#include "../../Loader/MIZLoader.h"
#include "Animation.h"

Animation::Animation(const string& filePath)
{
	data_ = MIZLoader::GetInstance().LoadAnimation(filePath.c_str());
}

void Animation::Release()
{
}
