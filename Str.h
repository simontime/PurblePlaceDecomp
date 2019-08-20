#include <Windows.h>
#include <wchar.h>

class Str
{
public:
	Str(unsigned int uID);
	Str(LPWSTR pwszSrc);
	Str(LPWSTR pwszSrc, int iLength);
	Str(Str const &Src);

	~Str();

	unsigned int AppendF(LPWSTR Format, ...);
	bool CIEquals(Str const &Target) const;
	void Clear();
	bool Equals(Str const &Target) const;
	unsigned int Format(LPWSTR Format, ...);
	static Str FromNumber(int iNumber);
	Str SubString(int cbOffset, int cbLength) const;

	Str &operator+=(Str const &Target);
	Str &operator+=(LPWSTR Target);
	Str &operator=(Str const &Target);
	Str &operator=(LPWSTR Target);

	unsigned int m_cbSize;
	unsigned int m_cbNullTerminatedSize;
	LPWSTR m_pwszStrBuf;
};