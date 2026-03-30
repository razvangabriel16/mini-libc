// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>

char *strcpy(char *destination, const char *source)
{
	char* d = destination;
	const char* s = source;
	for (;(*d++ = *s++);)
		;
	return destination;
}

char *strncpy(char *destination, const char *source, size_t len)
{
	size_t iter = 0;
	char* d = destination;
	for (; iter < len && *(source + iter) && (*(d + iter) = *(source + iter)); ++iter)
		;
	for (; iter < len; ++iter)	*(d + iter) = '\0';
	return destination;
}

char *strcat(char *destination, const char *source)
{
	size_t iter = 0, iter_after = 0;
	char* d = destination;
	for (; *(d + iter); ++iter)
		;
	for (; *(source + iter_after) && (*(d + iter) = *(source + iter_after)); ++iter, ++iter_after)
		;
	d[iter] = '\0';
	return destination;
}

char *strncat(char *destination, const char *source, size_t len)
{
	/* TODO: Implement strncat(). */
	size_t iter = 0, iter_after = 0;
	char* d = destination;
	for (; *(d + iter); ++iter)
		;
	for (; iter_after < len && *(source + iter_after) && (*(d + iter) = *(source + iter_after)); ++iter, ++iter_after)
		;
	d[iter] = '\0';
	return destination;
}

int strcmp(const char *str1, const char *str2)
{
	/* TODO: Implement strcmp(). */
	size_t iter = 0;
	for (; *(str1 + iter) && *(str2 + iter); iter++){
		if (str1[iter] < str2[iter]) return -1;
		else if (str1[iter] > str2[iter]) return 1;
	}
	if (*(str1 + iter)) return 1;
	if (*(str2 + iter)) return -1;
	return 0;
}

int strncmp(const char *str1, const char *str2, size_t len)
{
	size_t iter = 0;
	for (; iter < len && *(str1 + iter) && *(str2 + iter); iter++){
		if (str1[iter] < str2[iter]) return -1;
		else if (str1[iter] > str2[iter]) return 1;
	}
	if (iter < len && *(str1 + iter)) return 1;
	if (iter < len && *(str2 + iter)) return -1;
	return 0;
}

size_t strlen(const char *str)
{
	size_t i = 0;

	for (; *str != '\0'; str++, i++)
		;

	return i;
}

char *strchr(const char *str, int c)
{
	size_t iter = 0;
	for (;str[iter] && str[iter] != (int)c; ++iter)
		;
	return (str[iter] == (int)c) ? (char*)(str + iter) : NULL;
}

char* strrchr(const char *str, int c)
{
	long iter = 0;
	for (;str[iter++];)
		;
	if ((char)c == '\0')
        return (char*)(str + iter);
	iter--;
	do
        if (str[iter] == (char)c) return (char*)(str + iter);
    while (iter-- > 0)
		;
	return (str[iter] == (char)c) ? (char*)(str + iter) : NULL;
	return NULL;
}

char *strstr(const char *haystack, const char *needle)
{
	size_t iter_hay = 0, iter_needle;
	for (; haystack[iter_hay]; iter_hay++){
		iter_needle = 0;
		size_t iter_hay_cpy = iter_hay;
		unsigned char found = 1;
		for (; needle[iter_needle] && haystack[iter_hay_cpy]; iter_needle++, iter_hay_cpy++){
			if (needle[iter_needle] != haystack[iter_hay_cpy]){
				found = 0;
				break;
		}	}
        if (found) return (char*)(haystack + iter_hay);
	}
	return NULL;
}

char *strrstr(const char *haystack, const char *needle)
{
	size_t len_hay = 0;
	for (; haystack[len_hay]; len_hay++)
		;
	size_t len_needle = 0;
	for (; needle[len_needle]; len_needle++)
		;
	if (len_needle > len_hay) return NULL;
	//first done the comparison before the decrementation and then in
	//inner logic is used the decremented value in order to avoid UNDERFLOW
	for (size_t i = len_hay - len_needle + 1; i-- > 0;) {
        size_t j;
        for (j = 0; j < len_needle; ++j)
            if (haystack[i + j] != needle[j]) break;
        if (j == len_needle) return (char *)(haystack + i);
	}
	return NULL;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	unsigned char* d = (unsigned char*)destination;
	unsigned char* s = (unsigned char*)source;
	for (size_t iter = 0; iter < num && (*(d+iter) = *(s+iter)); ++iter)
		;
	return destination;
}

void *memmove(void *destination, const void *source, size_t num)
{
	for (size_t iter = 0; iter < num; ++iter) *((unsigned char*)destination + iter) = *((unsigned char*)source + iter)
		;
	return destination;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	size_t iter = 0;
	for (; iter < num && (*((unsigned char*)ptr1 + iter) == *((unsigned char*)ptr2 + iter)); iter++)
		;
	if (iter == num) return 0;
	return *((unsigned char*)(ptr1 + iter)) - *((unsigned char*)(ptr2 + iter));
}

void *memset(void *source, int value, size_t num)
{
//always use of unsigned char* casting over char* because
//of bytes comparison arithmetic. If one byte is 145 then char will consider it -111
	for(size_t iter = 0; iter < num; ++iter)
		*((unsigned char*)source + iter) = (unsigned char)value;
	return source;
}
