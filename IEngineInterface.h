#include <Windows.h>

class IEngineInterface
{
public:
	virtual const unsigned short *GetContextMenu();
	virtual LPCWSTR GetGdfPath();
	virtual void GetInitialLayout(unsigned int *x, unsigned int *y);
	virtual LPCWSTR GetResourceDllFileName();
	virtual void GetRichGameMediaGameID(GUID *guid);
	virtual LPCWSTR GetSplashTextFont();
	virtual int GetSplashTextFontSize();

private:
	const LPWSTR String = 0;
};