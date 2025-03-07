#include "Application/Application.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#ifdef _DEBUG
	// ÉÅÉÇÉäÉäÅ[ÉNåüèo
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	auto& application = Application::GetInstance();

	if (!application.Initialize())
	{
		return -1;
	}

	application.Execute();
	application.Terminate();

	return 0;
}
