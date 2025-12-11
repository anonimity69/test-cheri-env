// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#include <compartment.h>
#include <debug.hh>
#include <unwind.h>
#include <fail-simulator-on-error.h>

using Debug = ConditionalDebug<true, "Heap Buffer Over Write Compartment">;

int __cheri_compartment("heap-buffer-over-write") vuln1()
{
    Debug::log("Testing Heap Buffer Over-write (C++)...");
    int* arr = nullptr;
    arr = new int[3];
    if (!arr) {
        Debug::log("Allocation failed!");
        return 0;
    }
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    int idx = 10;
    Debug::log("Attempting to write arr[%d]...", idx);
    /* Bounds-check before writing to avoid heap buffer over-write and CHERI faults. */
    if (idx >= 0 && idx < 3) {
        arr[idx] = 999;
        Debug::log("arr[%d] set to {} (in-bounds)", idx, arr[idx]);
    } else {
        Debug::log("Index %d out of bounds (size 3), write avoided", idx);
    }
    delete[] arr;
    arr = nullptr;
    return 0;
}
