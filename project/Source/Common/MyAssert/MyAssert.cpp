#include "MyAssert.h"

void ErrorAssert(std::string_view errMsg, const char* file, int line)
{
	//Releaseにはwassertはない！
#ifdef _DEBUG

    // errMsg は string_view なので安全に std::string に変換
    std::wstring wErrStr = sjis_to_wide(std::string(errMsg));
    std::wstring wErrFile = sjis_to_wide(file);

    // 安全に行番号を渡す
    _wassert(wErrStr.c_str(), wErrFile.c_str(), static_cast<unsigned int>(line));
#endif // DEBUG
}
