#include "strchrnul.h"
#include <string.h>

char *strchrnul(const char *s, int c)
{
	/* Note: no test for NULL pointer here is intentional! strchrnul requires a non-null pointer s.
		On Linux, strchrnul() generates a segmentation fault if passed a NULL pointer.*/
	while (*s && *s != c) {
		++s;
	}
	return (char *)s;
}
