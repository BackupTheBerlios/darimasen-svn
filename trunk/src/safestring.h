/* A quick header file for the two functions. These where originally from
 * the OpenBSD project as strlcpy and strlcat. These exist because the
 * GNU Libc does not include strlcpy and strlcat like all other UINX-like
 * modern C libraries. They are made available under a BSD-style
 * license, and I encourage you to use this header file and the code
 * should you be so inclined. To this end, this header file is placed
 * into the public domain.
 *
 * If you find these functions useful, please complain to the GNU
 * foundation so that we may, someday, have a reliable, cross-platform
 * and safe C string API Lets make dangerous, unchecked buffer vulns
 * can be a thing of the past.
 *
 * - Ron Kuslak,2005
 */

#ifndef __SAFESTRING_H

#define __SAFESTRING_H
#include <sys/types.h>

size_t safe_strcpy(char *dst, const char *src, size_t siz);
size_t safe_strcat(char *dest, const char *src, size_t siz);

#endif /*  __SAFESTRING_H */
