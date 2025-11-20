#include "MyAssert.h"

void ErrorAssert(std::string_view errMsg, const char* file, int line)
{
	//Release‚É‚Íwassert‚Í‚È‚¢I
#ifdef _DEBUG

	//==•¶š—ñ‚ğƒƒCƒh•¶š—ñ‚É•ÏŠ·‚·‚é===
	std::wstring wErrStr = sjis_to_wide(errMsg.data());
	std::wstring wErrFile = sjis_to_wide(file);
	std::wstring wErrLine = sjis_to_wide(file);

	_wassert(wErrStr.data(), wErrFile.data(), (unsigned int)wErrLine.data());
#endif // DEBUG
}
