#ifndef HOOK_HPP
#define HOOK_HPP

#include <Windows.h>

#include "../venom/globals.hpp"

template<typename T>
struct hook
{
    bool success;
    int length;
    void *stolen;
    void *target;
    void *replacement;

    void make(T &target, void *replacement, int length)
    {
        this->target = target;
        this->replacement = replacement;
        this->length = length;
        this->success = false;
        
        if (this->length < 5)
            return;

        this->stolen = VirtualAlloc(0, this->length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        memcpy_s(this->stolen, this->length, this->target, this->length);

        *(BYTE *)((uintptr_t)this->stolen + this->length) = 0xE9;
        *(uintptr_t *)((uintptr_t)this->stolen + this->length + 1) = (uintptr_t)this->target - (uintptr_t)this->stolen - 5;

        this->success = true;
        target = (T) this->stolen;
    }

    void activate()
    {
        DWORD oldProtect;
        VirtualProtect(this->target, this->length, PAGE_EXECUTE_READWRITE, &oldProtect);

        *(BYTE *)this->target = 0xE9;
        *(uintptr_t *)((uintptr_t)this->target + 1) = (uintptr_t)this->replacement - (uintptr_t)this->target - 5;

        VirtualProtect(this->target, this->length, oldProtect, &oldProtect);
    }

    void restore()
    {
        DWORD oldProtect;
        VirtualProtect(this->target, this->length, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy_s(this->target, this->length, this->stolen, this->length);
        ZeroMemory(this->stolen, this->length);
        VirtualProtect(this->target, this->length, oldProtect, &oldProtect);
    }
};

#endif