// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.h>
#include <unwind.h>
#include <stdlib.h>

#define DEBUG_CONTEXT "Heap Buffer Under Write Compartment"

__cheri_compartment("heap-buffer-under-write") int vuln1(void)
{
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Heap Buffer Under-write (C)...");

    int *arr = (int *)malloc(3 * sizeof(int));
    if (arr == NULL) { return 0; }
    arr[0] = 10; arr[1] = 20; arr[2] = 30;

    int idx = -1;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Attempting under-write arr[%d]...", idx);
    /* Bounds-check to avoid writing before the allocation (under-write) and CHERI faults. */
    if (idx >= 0 && idx < 3) {
        arr[idx] = 999;
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "arr[%d]: {} (in-bounds)", idx, arr[idx]);
    } else {
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Index %d out of bounds (size 3), write avoided", idx);
    }

    free(arr);
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Freed array (if we reached here).");
    return 0;
}
