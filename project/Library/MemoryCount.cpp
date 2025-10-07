//#include "MemoryCount.h"
//#include <atomic>
//#include <cstdlib>
//#include <new>
//
////ウィンドウズを使っているときはこれが実行される。
//#ifdef _WIN32 
//#include <windows.h>
//#endif
//#pragma warning(disable:28251)  // 注釈不整合の警告番号　//どうしてもwaringが直らなかったのでこれで苦肉の策
//
//namespace {
//    std::atomic<size_t> totalMemorySize{ 0 };
//}
//
//// ====== operator new ======
//
//void* __CRTDECL operator new(std::size_t _size) noexcept(false) {
//    totalMemorySize += _size;//メモリのサイズを加算
//    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
//    void* _ptr = std::malloc(_size);
//    if (!_ptr) throw std::bad_alloc();
//    return _ptr;
//}
//
//void* __CRTDECL operator new(std::size_t _size, const std::nothrow_t&) noexcept {
//    totalMemorySize += _size;//メモリのサイズを加算
//    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
//    return std::malloc(_size);
//}
//
//void* __CRTDECL operator new[](std::size_t _size) noexcept(false) {
//    totalMemorySize += _size;//メモリのサイズを加算
//    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
//    void* _ptr = std::malloc(_size);
//    if (!_ptr) throw std::bad_alloc();
//    return _ptr;
//}
//
//void* __CRTDECL operator new[](std::size_t _size, const std::nothrow_t&) noexcept {
//    totalMemorySize += _size;//メモリのサイズを加算
//    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
//    return std::malloc(_size);
//}
//
//// ====== operator delete ======
//
//void operator delete(void* _ptr, std::size_t _size) noexcept {
//    // 解放するメモリサイズを減算
//    totalMemorySize -= _size;
//    // 実際のメモリ解放は free で行う(newの時と同じでこっちもしっかりと処理をしないといけない)
//    std::free(_ptr);
//}
//
//void operator delete(void* _ptr, const std::nothrow_t&) noexcept {
//    std::free(_ptr);
//}
//
//void operator delete[](void* _ptr, std::size_t _size) noexcept {
//    // 解放するメモリサイズを減算
//    totalMemorySize -= _size;
//    // 実際のメモリ解放は free で行う(newの時と同じでこっちもしっかりと処理をしないといけない)
//    std::free(_ptr);
//}
//
//void operator delete[](void* _ptr, const std::nothrow_t&) noexcept {
//    std::free(_ptr);
//}
//
//// ====== メモリ使用状況取得 ======
//
//size_t GetTrackedMemoryUsage() {
//    return totalMemorySize.load();
//}
//
//size_t GetTotalPhysicalMemory() {
//#ifdef _WIN32
//    MEMORYSTATUSEX _memInfo;
//    _memInfo.dwLength = sizeof(MEMORYSTATUSEX);
//    if (GlobalMemoryStatusEx(&_memInfo)) {
//        return static_cast<size_t>(_memInfo.ullTotalPhys);
//    }
//
//#endif
//    return 0;
//}
//
//#pragma warning(pop)

#include "MemoryCount.h"
#include <atomic>
#include <cstdlib>
#include <new>
#include <cstdint>
#include <iostream>

////ウィンドウズを使っているときはこれが実行される。
#ifdef _WIN32
#include <windows.h>
#endif
// 注釈不整合の警告番号　//どうしてもwaringが直らなかったのでこれで苦肉の策
#pragma warning(disable:28251)
#include <DxLib.h>

namespace {
    std::atomic<size_t> totalMemorySize{ 10000000 };//合計のメモリサイズ
    static const  size_t HEADER_SIZE = sizeof(size_t); // 先頭に保存するサイズの量
}

void* operator new(std::size_t size) {
    size_t s = HEADER_SIZE;
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    //メモリに格納されなかったらエラーを流す。
    if (!raw) {
        throw std::bad_alloc();
    }
    // 先頭にサイズ情報をいれる
    *static_cast<size_t*>(raw) = size;
   totalMemorySize += size;//メモリのサイズを加算
    return static_cast<char*>(raw) + HEADER_SIZE;
}

void* operator new[](std::size_t size) {
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    if (!raw) {
        throw std::bad_alloc();
    }

    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;//メモリのサイズを加算

    return static_cast<char*>(raw) + HEADER_SIZE;
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    if (!raw) {
        return nullptr;
    }

    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;

    return static_cast<char*>(raw) + HEADER_SIZE;
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    if (!raw) {
        return nullptr;
    }

    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;

    return static_cast<char*>(raw) + HEADER_SIZE;
}

void tracked_delete(void* ptr) {
    if (!ptr) return;
    void* raw = static_cast<char*>(ptr) - HEADER_SIZE;
    size_t size = *static_cast<size_t*>(raw);
    //totalMemorySize -= size;
   // printfDx("[DEL] ptr=%p size=%u before=%u\n", ptr, (unsigned int) size, (unsigned int)totalMemorySize.load());
    //ScreenFlip();
    totalMemorySize -= size;
   // printfDx("[DEL] after=%u\n", (unsigned int)totalMemorySize.load());
    std::free(raw);
}

void operator delete(void* ptr) noexcept {
    tracked_delete(ptr);
}
void operator delete[](void* ptr) noexcept {
    tracked_delete(ptr);
}
void operator delete(void* ptr, std::size_t) noexcept {
    tracked_delete(ptr); // 安全に統一
}
void operator delete[](void* ptr, std::size_t) noexcept {
    tracked_delete(ptr);
}

size_t GetMemory()
{
    return totalMemorySize.load();
}

size_t GetTotalMemory()
{
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return static_cast<size_t>(memInfo.ullTotalPhys);
    }
#endif
    return 0;
}
