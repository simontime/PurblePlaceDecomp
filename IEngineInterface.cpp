#include "IEngineInterface.h"

const unsigned short *IEngineInterface::GetContextMenu() 
{
	return 0;
}

LPCWSTR IEngineInterface::GetGdfPath()
{
	return 0;
}

void IEngineInterface::GetInitialLayout(unsigned int *x, unsigned int *y)
{
	*x = 800;
	*y = 496;
}

LPCWSTR IEngineInterface::GetResourceDllFileName()
{
	return L"files.dll";
}

void IEngineInterface::GetRichGameMediaGameID(GUID *guid)
{
	*guid = GUID_NULL;
}

LPCWSTR IEngineInterface::GetSplashTextFont()
{
	return L"Segoe UI";
}

int IEngineInterface::GetSplashTextFontSize()
{
	return 60;
}
