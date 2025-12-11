// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#include <compartment.h>
#include <debug.hh>
#include <unwind.h>
#include <fail-simulator-on-error.h>

using Debug = ConditionalDebug<true, "Use After Free Compartment">;

int __cheri_compartment("use-after-free") vuln1()
{
    Debug::log("Testing Use-After-Free (C++)...");
    int* ptr = new int;
    if (!ptr)
    {
        Debug::log("Allocation failed!");
        return 0;
    }
    *ptr = 123;
    Debug::log("ptr capability: {}", ptr);
    Debug::log("ptr points to memory with value: {}", *ptr);
    delete ptr;
    ptr = nullptr;
    Debug::log("Memory has been freed and pointer nullified.");
    /* Avoid use-after-free by checking if pointer is null before dereferencing. */
    if (ptr != nullptr) {
        *ptr = 456;
        Debug::log("Value is now: {}", *ptr);
    } else {
        Debug::log("Pointer is nullptr, skipping dereference to avoid use-after-free.");
    }
    return 0;
}
