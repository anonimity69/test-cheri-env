// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.h>
#include <unwind.h>
#include <stdlib.h>


#define DEBUG_CONTEXT "Use After Free Compartment"

/// Thread entry point.
__cheri_compartment("use-after-free") int vuln1()
{
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Use-After-Free (C)...");
    int* ptr = (int*)malloc(sizeof(int));
    if (ptr == NULL) {return 0;}
    *ptr = 123;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "ptr points to memory with value: {}", *ptr);

    free(ptr);
    ptr = NULL;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Memory has been freed and pointer nullified.");

    /* Avoid use-after-free by checking if pointer is null before dereferencing. */
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Checking pointer before dereference...");
    if (ptr != NULL) {
        *ptr = 456;
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Value is now: {}", *ptr);
    } else {
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Pointer is NULL, skipping dereference to avoid use-after-free.");
    }

    return 0;
}
