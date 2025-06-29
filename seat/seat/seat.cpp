#include "seat.h"
#include <vector>
#include <iostream>
#include <Windows.h>


void* seat::Hook( void* instance, unsigned int index, void* function ) {
	if (!instance || !function) return nullptr;
    
    /* Get vtable from the instance */
    void** vtable = *(void***)instance;

    /* Where we store all of the original functions */
    std::vector<void*> vtableFunctions;

    /* Go through the whole vtable until we reach the end */
    int i = 0;
    while (true) {
        void* func = vtable[i];
        if (!func)
            break;

        MEMORY_BASIC_INFORMATION mbi = {};
        if (!VirtualQuery(func, &mbi, sizeof(mbi)))
            break;

        DWORD prot = mbi.Protect;
        bool exec = prot & (PAGE_EXECUTE | PAGE_EXECUTE_READ |
                            PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);

        if (!exec)
            break;

        vtableFunctions.push_back(func);
        i++;
    }

    /* Create our own vtable to replace the original */
    void** replacementVtable = new void* [vtableFunctions.size()];
    int ii = 0;
    for (auto* fn : vtableFunctions) {
        replacementVtable[ii] = fn;
        ii++;
    }

    /* Store original function to call later */
    void* originalFunction = replacementVtable[index];

    /* Replace the vfunc in the replacement vtable */
    replacementVtable[index] = function;

    /* Replace the vtable*/
    *reinterpret_cast<void***>(instance) = replacementVtable;
    
    /* Return original so that you can call it later */
    return originalFunction;
}

/*
*   I can't really decide how to do this
*   This is undeniably an ugly method but I'll make it pretty in a few days when I can be asked again (or am forced to do so because I want to be able to unload my cheat)
*/
bool seat::Unhook(void* instance, void** replacementVtable, void** originalVtable) {
    if (!instance || !replacementVtable || !originalVtable) return false;

    DWORD oldProtect;
    void** instanceVtablePtr = reinterpret_cast<void**>(instance);
    if (!VirtualProtect(instanceVtablePtr, sizeof(void*), PAGE_READWRITE, &oldProtect))
        return false;

    *reinterpret_cast<void***>(instance) = originalVtable;

    VirtualProtect(instanceVtablePtr, sizeof(void*), oldProtect, &oldProtect);

    delete[] replacementVtable;

    return true;
}