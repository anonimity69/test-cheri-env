// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.h>
#include <unwind.h>
#include <stdlib.h>


#define DEBUG_CONTEXT "Heap Buffer Over Read Compartment"

/// Thread entry point.
__cheri_compartment("heap-buffer-over-read") int vuln1()
{
    int* arr = (int*)malloc(3 * sizeof(int));
    if (arr == NULL) {return 0;} // Always check for malloc failure
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Buffer Over-read (C)...");

    int idx = 10;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Accessing arr[%d]... ", idx);
    int value = 0;
    if (idx >= 0 && idx < 3) {
        value = arr[idx];
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Value: {} (in-bounds)", value);
    } else {
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Index %d out of bounds (size 3), read avoided", idx);
    }

    free(arr);
    arr = NULL;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "This line may not be reached if the program crashes.");

    return 0;
}
