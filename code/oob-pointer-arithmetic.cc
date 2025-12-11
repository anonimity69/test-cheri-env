// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#include <compartment.h>
#include <debug.hh>
#include <unwind.h>
#include <fail-simulator-on-error.h>

using Debug = ConditionalDebug<true, "OOB Pointer Arithmetic Compartment">;

int __cheri_compartment("oob-pointer-arithmetic") vuln1()
{
    Debug::log("Testing Out-Of-Bounds Pointer Arithmetic (C++)...");
    int arr[4] = {100, 200, 300, 400};
    Debug::log("Array base: {}", static_cast<void*>(arr));
    int idx = 10;
    int* p = arr + idx;
    Debug::log("Pointer moved to arr + %d: {}", idx, static_cast<void*>(p));
    /* Bounds-check before dereferencing pointer produced by arithmetic. */
    if (idx >= 0 && idx < static_cast<int>(sizeof(arr)/sizeof(arr[0]))) {
        int val = *p;
        Debug::log("Read value: {} (in-bounds)", val);
    } else {
        Debug::log("Index %d out of bounds (size %d), access avoided", idx, static_cast<int>(sizeof(arr)/sizeof(arr[0])));
    }
    return 0;
}
