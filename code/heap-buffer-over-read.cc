// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT


#include <compartment.h>
#include <debug.hh>
#include <unwind.h>
#include <fail-simulator-on-error.h>

/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "Heap Buffer Over Read Compartment">;


/// Thread entry point.
int __cheri_compartment("heap-buffer-over-read") vuln1()
{
    Debug::log("Running Buffer Over-read (C++)...");
    int* arr = new int[3];
    if (arr == nullptr)
    {
        Debug::log("Allocation failed!");
        return -1;
    }
    Debug::log("Array created, assigning values...");
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    int idx = 10;
    Debug::log("Accessing arr[%d]...", idx);
    int value = 0;
    /* Bounds-check to avoid heap buffer over-read and CHERI faults. */
    if (idx >= 0 && idx < 3) {
        value = arr[idx];
        Debug::log("Value: {} (in-bounds)", value);
    } else {
        Debug::log("Index %d out of bounds (size 3), read avoided", idx);
    }
    delete[] arr;
    arr = nullptr;
    Debug::log("Array deleted and pointer nullified");
    return 0;
}
