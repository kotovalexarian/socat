#include "../config.h"
#include "../xioconfig.h"

#include "cv_newline.h"

#include "../sycls.h"
#include "../xio.h"

/* converts the newline characters (or character sequences) from the one
 * specified in lineterm1 to that of lineterm2. Possible values are
 * LINETERM_CR, LINETERM_CRNL, LINETERM_RAW.
 * buff points to the malloc()'ed data, input and output. It may be subject to
 * realloc(). bytes specifies the number of bytes input and output
 */
int cv_newline(
    unsigned char **buff,
    ssize_t *bufsiz,
    int lineterm1,
    int lineterm2,
    size_t socat_opts_bufsiz
) {
    ssize_t *bytes = bufsiz;
    /* must perform newline changes */
    if (lineterm1 <= LINETERM_CR && lineterm2 <= LINETERM_CR) {
        /* no change in data length */
        unsigned char from, to,  *p, *z;
        if (lineterm1 == LINETERM_RAW) {
            from = '\n';
            to = '\r';
        } else {
            from = '\r';
            to = '\n';
        }
        z = *buff + *bytes;
        p = *buff;
        while (p < z) {
            if (*p == from) {
                *p = to;
            }
            ++p;
        }

    } else if (lineterm1 == LINETERM_CRNL) {
        /* buffer becomes shorter */
        unsigned char to, *s, *t, *z;
        if (lineterm2 == LINETERM_RAW) {
            to = '\n';
        } else {
            to = '\r';
        }
        z = *buff + *bytes;
        s = t = *buff;
        while (s < z) {
            if (*s == '\r') {
                ++s;
                continue;
            }
            if (*s == '\n') {
                *t++ = to;
                ++s;
            } else {
                *t++ = *s++;
            }
        }
        *bufsiz = t - *buff;

    } else {
        /* buffer becomes longer, must alloc another space */
        unsigned char *buf2;
        unsigned char from;
        unsigned char *s, *t, *z;
        if (lineterm1 == LINETERM_RAW) {
            from = '\n';
        } else {
            from = '\r';
        }
        buf2 = Malloc(2 * socat_opts_bufsiz /*sic!*/ + 1);
        if ((buf2) == NULL) {
            return -1;
        }
        s = *buff;
        t = buf2;
        z = *buff + *bytes;
        while (s < z) {
            if (*s == from) {
                *t++ = '\r';
                *t++ = '\n';
                ++s;
                continue;
            } else {
                *t++ = *s++;
            }
        }
        free(*buff);
        *buff = buf2;
        *bufsiz = t - buf2;;
    }
    return 0;
}
