#include "Str.h"

Str::Str(unsigned int uID)
{
	int cbLoadedSize, cbBufferSize = 1024;

	m_cbSize = m_cbNullTerminatedSize = 0;
	m_pwszStrBuf = nullptr;

	for (;;)
	{
		m_pwszStrBuf = new WCHAR[cbBufferSize];

		if (m_pwszStrBuf == nullptr)
			break;

		cbLoadedSize = LoadStringW(GetModuleHandleW(NULL), uID, m_pwszStrBuf, cbBufferSize);
		
		delete[] m_pwszStrBuf;

		if (cbLoadedSize < cbBufferSize - 1)
		{
			m_cbSize = cbLoadedSize;

			if (!m_cbSize)
				return;

			m_cbNullTerminatedSize = m_cbSize + 1;

			m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];

			if (m_pwszStrBuf)
			{
				LoadStringW(GetModuleHandleW(NULL), uID, m_pwszStrBuf, m_cbSize + 1);
				return;
			}

			break;
		}

		cbBufferSize *= sizeof(WCHAR);

		if (cbBufferSize > 0x100000)
		{
			m_cbSize = 0xFFFFF;

			m_cbNullTerminatedSize = m_cbSize + 1;

			m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];

			if (m_pwszStrBuf)
			{
				LoadStringW(GetModuleHandleW(NULL), uID, m_pwszStrBuf, m_cbSize + 1);
				return;
			}

			break;
		}
	}

	m_cbNullTerminatedSize = 0;
	m_cbSize = 0;
}

Str::Str(LPWSTR pwszSrc)
{
	if (pwszSrc == nullptr)
	{
		m_pwszStrBuf = nullptr;
		m_cbSize = m_cbNullTerminatedSize = 0;
		return;
	}

	m_cbSize = wcslen(pwszSrc);
	m_cbNullTerminatedSize = m_cbSize + 1;

	m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];

	if (m_pwszStrBuf == nullptr)
	{
		m_cbSize = m_cbNullTerminatedSize = 0;
		return;
	}

	wcscpy_s(m_pwszStrBuf, m_cbNullTerminatedSize, pwszSrc);
}

Str::Str(LPWSTR pwszSrc, int cbLength)
{
	if (pwszSrc == nullptr)
	{
		m_pwszStrBuf = nullptr;
		m_cbSize = m_cbNullTerminatedSize = 0;
		return;
	}
	
	m_cbSize = cbLength;
	m_cbNullTerminatedSize = m_cbSize + 1;

	m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];

	if (m_pwszStrBuf == nullptr)
	{
		m_cbSize = m_cbNullTerminatedSize = 0;
		return;
	}

	wcsncpy_s(m_pwszStrBuf, m_cbNullTerminatedSize, pwszSrc, m_cbSize);
}

Str::Str(Str const &Src)
{
	m_cbSize = Src.m_cbSize;
	m_cbNullTerminatedSize = m_cbSize + 1;

	m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];

	if (m_pwszStrBuf)
	{
		wcscpy_s(m_pwszStrBuf, m_cbNullTerminatedSize, Src.m_pwszStrBuf);
	}
	else
	{
		m_cbNullTerminatedSize = 0;
		m_cbSize = 0;
	}
}

Str::~Str()
{
	delete[] m_pwszStrBuf;
}

unsigned int Str::AppendF(LPWSTR Format, ...)
{
	unsigned int cbOldSize;
	LPWSTR pwszStrBuf;

	va_list ArgList;
	va_start(ArgList, Format);

	cbOldSize = m_cbSize;
	m_cbSize += _vscwprintf(Format, ArgList);

	if (m_cbNullTerminatedSize > m_cbSize)
		_vsnwprintf_s(&m_pwszStrBuf[cbOldSize], m_cbSize + 1, m_cbSize, Format, ArgList);
	else
	{
		pwszStrBuf = new WCHAR[m_cbSize];

		if (pwszStrBuf)
		{
			m_cbNullTerminatedSize = m_cbSize + 1;

			if (m_pwszStrBuf)
				wcscpy_s(pwszStrBuf, m_cbNullTerminatedSize, m_pwszStrBuf);
			else
				*pwszStrBuf = 0;

			_vsnwprintf_s(&pwszStrBuf[cbOldSize], m_cbSize + 1, m_cbSize, Format, ArgList);

			delete[] m_pwszStrBuf;

			m_pwszStrBuf = pwszStrBuf;
		}
	}

	return m_cbSize;
}

bool Str::CIEquals(Str const &Target) const
{
	if (Target.m_pwszStrBuf && m_pwszStrBuf)
		return _wcsicmp(Target.m_pwszStrBuf, m_pwszStrBuf) == 0;
	else
		return false;
}

void Str::Clear()
{
	delete[] m_pwszStrBuf;

	m_cbSize = m_cbNullTerminatedSize = 0;
	m_pwszStrBuf = nullptr;
}

bool Str::Equals(Str const &Target) const
{
	if (Target.m_pwszStrBuf && m_pwszStrBuf)
		return wcscmp(Target.m_pwszStrBuf, m_pwszStrBuf) == 0;
	else
		return false;
}

unsigned int Str::Format(LPWSTR Format, ...)
{
	va_list ArgList;
	va_start(ArgList, Format);

	m_cbSize = _vscwprintf(Format, ArgList);

	if (m_cbNullTerminatedSize <= m_cbSize)
	{
		delete[] m_pwszStrBuf;

		m_cbNullTerminatedSize = m_cbSize + 1;
		m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];
	}

	if (m_pwszStrBuf)
		_vsnwprintf_s(m_pwszStrBuf, m_cbSize + 1, m_cbSize, Format, ArgList);
	else
		m_cbSize = m_cbNullTerminatedSize = 0;

	return m_cbSize;
}

Str Str::FromNumber(int iNumber)
{
	WCHAR Dst[128];

	swprintf_s(Dst, 128, L"%i", iNumber);
	return Str(Dst);
}

Str Str::SubString(int cbOffset, int cbLength) const
{
	int cbNewLength = cbLength;

	if (cbOffset < 0 || cbLength < 0 || m_cbSize < cbOffset + cbLength)
		cbNewLength = m_cbSize - cbOffset;

	return Str(&m_pwszStrBuf[cbOffset], cbNewLength);
}

Str &Str::operator+=(Str const &Target)
{
	LPWSTR pwszStrBuf;

	this->m_cbSize += Target.m_cbSize;

	if (m_cbNullTerminatedSize > m_cbSize)
		wcscat_s(m_pwszStrBuf, m_cbNullTerminatedSize, Target.m_pwszStrBuf);
	else
	{
		pwszStrBuf = new WCHAR[m_cbSize + 1];

		if (pwszStrBuf)
		{
			m_cbNullTerminatedSize = m_cbSize + 1;

			if (m_pwszStrBuf)
				wcscpy_s(pwszStrBuf, m_cbNullTerminatedSize, m_pwszStrBuf);
			else
				*pwszStrBuf = 0;

			wcscat_s(pwszStrBuf, m_cbNullTerminatedSize, Target.m_pwszStrBuf);

			delete[] m_pwszStrBuf;

			m_pwszStrBuf = pwszStrBuf;
		}
	}

	return *this;
}

Str &Str::operator+=(LPWSTR Target)
{
	return *this += Str(Target);
}

Str &Str::operator=(Str const &Target)
{
	unsigned int cbTargetLength = wcslen(Target.m_pwszStrBuf);

	if (m_cbNullTerminatedSize <= cbTargetLength)
	{
		delete[] m_pwszStrBuf;

		m_cbNullTerminatedSize = cbTargetLength + 1;
		m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];
	}

	if (m_pwszStrBuf)
	{
		m_cbSize = cbTargetLength;
		wcscpy_s(m_pwszStrBuf, m_cbNullTerminatedSize, Target.m_pwszStrBuf);
	}

	return *this;
}

Str &Str::operator=(LPWSTR Target)
{
	unsigned int cbTargetLength = wcslen(Target);

	if (m_cbNullTerminatedSize <= cbTargetLength)
	{
		delete[] m_pwszStrBuf;

		m_cbNullTerminatedSize = cbTargetLength + 1;
		m_pwszStrBuf = new WCHAR[m_cbNullTerminatedSize];
	}

	if (m_pwszStrBuf)
	{
		m_cbSize = cbTargetLength;
		wcscpy_s(m_pwszStrBuf, m_cbNullTerminatedSize, Target);
	}

	return *this;
}