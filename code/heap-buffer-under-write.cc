// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#include <compartment.h>
#include <debug.hh>
#include <unwind.h>
#include <fail-simulator-on-error.h>

using Debug = ConditionalDebug<true, "Heap Buffer Under Write Compartment">;

int __cheri_compartment("heap-buffer-under-write") vuln1()
{
    Debug::log("Testing Heap Buffer Under-write (C++)...");
    int* arr = new int[3];
    if (!arr)
    {
        Debug::log("Allocation failed!");
        return 0;
    }
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    int idx = -1;
    Debug::log("Attempting under-write arr[%d]...", idx);
    /* Bounds-check to avoid writing before the allocation (under-write) and CHERI faults. */
    if (idx >= 0 && idx < 3) {
        arr[idx] = 999;
        Debug::log("arr[%d]: {} (in-bounds)", idx, arr[idx]);
    } else {
        Debug::log("Index %d out of bounds (size 3), write avoided", idx);
    }
    delete[] arr;
    arr = nullptr;
    return 0;
}
