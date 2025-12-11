#include <compartment.h>
#include <debug.h>
#include <unwind.h>

#define DEBUG_CONTEXT "Type Confusion Compartment"

const char hello[] = "Hello World!";

/* Use a tagged representation to avoid undefined type-punning and
   accidental pointer bit-manipulation that can cause CHERI faults. */
struct long_or_ptr {
    int is_ptr; /* non-zero when `ptr` is active */
    long l;
    const char *ptr;
} lp = { .is_ptr = 1, .ptr = hello };

void inc_long_ptr(struct long_or_ptr *lpp) {
    if (lpp->is_ptr && lpp->ptr != NULL) {
        /* advance the pointer one character (safe pointer arithmetic) */
        lpp->ptr++;
    } else {
        /* operate on the integer field when pointer is not active */
        lpp->l++;
    }
}

__cheri_compartment("type-confusion") int vuln1(void)
{
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Testing Type confusion (C)...");

    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "Before inc_long_ptr: lp.ptr = {}", (char*)lp.ptr);
    inc_long_ptr(&lp);
    CHERIOT_DEBUG_LOG(DEBUG_CONTEXT, "After inc_long_ptr: lp.ptr = {}", (char*)lp.ptr);

    return 0;
}
