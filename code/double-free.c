// Diagnostics for double-free behavior
// SPDX-License-Identifier: MIT

#include <errno.h>
#include <compartment.h>
#include <debug.h>
#include <unwind.h>
#include <stdlib.h>

#define DEBUG_CONTEXT "Double Free Compartment"

__cheri_compartment("double-free") int vuln1(void)
{
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Double Free...");

    int *ptr = (int*)malloc(sizeof(int));
    if (!ptr) { CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "malloc returned NULL"); return 0; }
    *ptr = 42;

    int *ptr2 = ptr;

    free(ptr);
    ptr = NULL;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "After first free; ptr nullified");

    /* Avoid double-free: ptr2 pointed to the same allocation which is already freed. */
    ptr2 = NULL;
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "ptr2 nullified to prevent double free");

    return 0;
}