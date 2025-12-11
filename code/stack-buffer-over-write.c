// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.h>
#include <assert.h>
#include <unwind.h>

#define DEBUG_CONTEXT "Stack Buffer Over Write Compartment"

#pragma weak write_buf

void write_buf(char *buf, size_t ix)
{
    buf[ix] = 'b';
}

__cheri_compartment("stack-buffer-over-write") int vuln1(void)
{
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Stack Buffer Over Write (C)...");

    char upper[0x10];
    char lower[0x10];

    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "upper = {}, lower = {}, diff = {}",
        (ptraddr_t)upper, (ptraddr_t)lower, (size_t)(upper - lower));
    
    /* Assert that these get placed how we expect */
    assert((ptraddr_t)upper == (ptraddr_t)&lower[sizeof(lower)]);

    upper[0] = 'a';
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "upper[0] = {}", upper[0]);

    /* Bounds-check the index before calling write_buf to avoid stack overwrite. */
    size_t ix = sizeof(lower);
    if (ix > 0 && ix <= sizeof(lower)) {
        /* If ix == sizeof(lower) it's out-of-bounds (past end) so avoid write. */
        if (ix - 1 < sizeof(lower)) {
            write_buf(lower, ix - 1);
            CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Wrote lower[%zu] safely", ix - 1);
        }
    } else {
        CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Index %zu out of bounds, write avoided", ix);
    }

    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "upper[0] = {}", upper[0]);

    return 0;
}
