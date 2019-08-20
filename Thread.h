#include <Windows.h>

class Thread
{
public:
	Thread();
	~Thread();

	virtual void Begin();
	virtual void End();
	virtual bool IsRunning() const;

	HANDLE m_hHandle;
	DWORD m_dwThreadId;
	bool m_bIsRunning;
};