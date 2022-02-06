#pragma once
#include <unordered_set>
#include "waObj.h"
class animEventHandler {
public:
    template<class Ty>
    Ty SafeWrite64Function(uintptr_t addr, Ty data) {
        DWORD oldProtect;
        void* _d[2];
        memcpy(_d, &data, sizeof(data));
        size_t len = sizeof(_d[0]);

        VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
        Ty olddata;
        memset(&olddata, 0, sizeof(Ty));
        memcpy(&olddata, (void*)addr, len);
        memcpy((void*)addr, &_d[0], len);
        VirtualProtect((void*)addr, len, oldProtect, &oldProtect);
        return olddata;
    }

    typedef RE::BSEventNotifyControl(animEventHandler::* FnProcessEvent)(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher);

    RE::BSEventNotifyControl HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src);

    void HookSink(uintptr_t ptr) {
        FnProcessEvent fn = SafeWrite64Function(ptr + 0x8, &animEventHandler::HookedProcessEvent);
        fnHash.insert(std::pair<uint64_t, FnProcessEvent>(ptr, fn));
    }


    static animEventHandler* GetSingleton()
    {
        static animEventHandler singleton;
        return  std::addressof(singleton);
    }

protected:
    static std::unordered_map<uint64_t, FnProcessEvent> fnHash;
};

