// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#include <compartment.h>
#include <debug.hh>
#include <unwind.h>
#include <fail-simulator-on-error.h>

using Debug = ConditionalDebug<true, "Type Confusion Compartment">;

const char Hello[] = "Hello World!";

/* Use a tagged representation to avoid undefined type-punning and
   accidental pointer bit-manipulation that can cause CHERI faults. */
struct long_or_ptr {
    int is_ptr; /* non-zero when `ptr` is active */
    long l;
    const char* ptr;
} lp = { .is_ptr = 1, .ptr = Hello };

void inc_long_ptr(long_or_ptr* lpp) {
    if (lpp->is_ptr && lpp->ptr != nullptr) {
        /* advance the pointer one character (safe pointer arithmetic) */
        lpp->ptr++;
    } else {
        /* operate on the integer field when pointer is not active */
        lpp->l++;
    }
}

int __cheri_compartment("type-confusion") vuln1()
{
    Debug::log("Testing Type confusion (C++)...");
    Debug::log("Before inc_long_ptr: lp.ptr = {}", lp.ptr);
    inc_long_ptr(&lp);
    Debug::log("After inc_long_ptr: lp.ptr = {}", lp.ptr);
    return 0;
}
