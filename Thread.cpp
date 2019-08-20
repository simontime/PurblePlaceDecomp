#include "Thread.h"

// TO-DO: Fix this atrocity
void StartAddress(LPVOID lpThreadParameter)
{
	(*(void(**)())(*(INT_PTR *)lpThreadParameter + 4))();
}

Thread::Thread()
{
	m_hHandle = nullptr;
	m_dwThreadId = 0;
	m_bIsRunning = false;
}

Thread::~Thread()
{
	End();
}

void Thread::Begin()
{
	if (m_hHandle)
		End();

	m_bIsRunning = true;
	m_hHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAddress, this, 0, &m_dwThreadId);
}

void Thread::End()
{
	if (m_hHandle)
	{
		m_bIsRunning = false;
		WaitForSingleObject(m_hHandle, -1);
		CloseHandle(m_hHandle);
		m_hHandle = nullptr;
	}
}

bool Thread::IsRunning() const
{
	return m_bIsRunning;
}

