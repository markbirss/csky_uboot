#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

void * memcpy(void *dest, const void *src, size_t count)
{
    if (dest == NULL || src == NULL) {
        return NULL;
    }
    int i;
    char *pdest;
    const char *psrc;
    int n = count;

    if (pdest > psrc && pdest < psrc + count) {
        for (i = n-1;i != -1; --i) {
            pdest[i] = psrc[i];
        }
    } else {
        for (i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    }
    return dest;
}

void * memset(void * s,int c,size_t count)
{
    unsigned long *sl = (unsigned long *) s;
    unsigned long cl = 0;
    char *s8;
    int i;

    /* do it one word at a time (32 bits or 64 bits) while possible */
    if ( ((ulong)s & (sizeof(*sl) - 1)) == 0) {
        for (i = 0; i < sizeof(*sl); i++) {
            cl <<= 8;
            cl |= c & 0xff;
        }
        while (count >= sizeof(*sl)) {
            *sl++ = cl;
            count -= sizeof(*sl);
        }    
    }                          
    /* fill 8 bits at a time */
    s8 = (char *)sl;
    while (count--)
        *s8++ = c;
    return s;
}
