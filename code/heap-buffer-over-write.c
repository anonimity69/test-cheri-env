// Copyright Microsoft and CHERiOT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.h>
#include <unwind.h>
#include <stdlib.h>

#define DEBUG_CONTEXT "Heap Buffer Over Write Compartment"

__cheri_compartment("heap-buffer-over-write") int vuln1(void)
{
    int* arr = (int*)malloc(3 * sizeof(int));
    if (arr == NULL) { return 0; }

    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Buffer Over-write (C)...");

    int idx = 4;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Attempting to write arr[%d]...", idx);
    /* Bounds-check before writing to avoid heap buffer over-write and CHERI faults. */
    if (idx >= 0 && idx < 3) {
        arr[idx] = 999;
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "arr[%d]: {} (in-bounds)", idx, arr[idx]);
    } else {
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Index %d out of bounds (size 3), write avoided", idx);
    }

    free(arr);
    arr = NULL;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Array freed and pointer nullified");

    return 0;
}
