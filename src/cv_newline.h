/* source: src/cv_newline.h */
/* Copyright Gerhard Rieger and contributors (see file CHANGES) */
/* Published under the GNU General Public License V.2, see file COPYING */

#ifndef __cv_newline_h_included
#define __cv_newline_h_included 1

#include "../sysincludes.h"

int cv_newline(
    unsigned char **buff,
    ssize_t *bytes,
    int lineterm1,
    int lineterm2,
    size_t socat_opts_bufsiz
);

#endif // !defined(__cv_newline_h_included)
