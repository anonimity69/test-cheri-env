// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.h>
#include <unwind.h>

#define DEBUG_CONTEXT "OOB Pointer Arithmetic Compartment"

__cheri_compartment("oob-pointer-arithmetic") int vuln1(void)
{
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Out-Of-Bounds Pointer Arithmetic (C)...");
    int arr[4] = {100, 200, 300, 400};
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Array base: {}", (uintptr_t)arr);

    /* Demonstrate pointer arithmetic safely by checking bounds first. */
    int idx = 10;
    int *p = arr + idx;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Pointer moved to arr + %d: {}", idx, (uintptr_t)p);

    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Attempting to dereference pointer at arr + %d...", idx);
    if (idx >= 0 && idx < (int)(sizeof(arr)/sizeof(arr[0]))) {
        int val = *p; /* safe in-bounds access */
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Read value: {} (in-bounds)", val);
    } else {
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Index %d out of bounds (size %d), access avoided", idx, (int)(sizeof(arr)/sizeof(arr[0])));
    }

    return 0;
}
