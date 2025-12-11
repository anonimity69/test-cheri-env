// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#include <compartment.h>
#include <debug.hh>
#include <assert.h>
#include <unwind.h>
#include <fail-simulator-on-error.h>

using Debug = ConditionalDebug<true, "Stack Buffer Over Write Compartment">;

void write_buf(char* buf, size_t ix)
{
    buf[ix] = 'b';
}

int __cheri_compartment("stack-buffer-over-write") vuln1()
{
    Debug::log("Testing Stack Buffer Over Write (C++)...");
    char upper[0x10];
    char lower[0x10];
    Debug::log(
        "upper = {}, lower = {}, diff = {}",
        static_cast<void*>(upper),
        static_cast<void*>(lower),
        static_cast<long>(reinterpret_cast<uintptr_t>(upper) - reinterpret_cast<uintptr_t>(lower))
    );
    assert((uintptr_t)upper == (uintptr_t)&lower[sizeof(lower)]);
    upper[0] = 'a';
    Debug::log("upper[0] = {}", upper[0]);
    /* Avoid passing an out-of-bounds index to write_buf. */
    size_t ix = sizeof(lower);
    if (ix > 0 && ix <= sizeof(lower)) {
        /* ix == sizeof(lower) is past-the-end; write into last element instead. */
        size_t safe_ix = (ix == sizeof(lower)) ? (ix - 1) : ix - 1;
        write_buf(lower, safe_ix);
        Debug::log("Wrote lower[%zu] safely", safe_ix);
    } else {
        Debug::log("Index %zu out of bounds, write avoided", ix);
    }
    Debug::log("upper[0] = {}", upper[0]);
    return 0;
}
