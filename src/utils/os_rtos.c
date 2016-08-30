/*
 * wpa_supplicant/hostapd / Real time OS specific functions
 * Copyright (c) 2016, Kelvin Cheung <keguang.zhang@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */

#include "includes.h"

#include "os.h"
#include "common.h"

void os_sleep(os_time_t sec, os_time_t usec)
{
	if (sec)
		sleep(sec);
	if (usec)
		usleep(usec);
}


int os_get_time(struct os_time *t)
{
	int res;
	struct timeval tv;
	res = gettimeofday(&tv, NULL);
	t->sec = tv.tv_sec;
	t->usec = tv.tv_usec;
	return res;
}


int os_get_reltime(struct os_reltime *t)
{
	int res;
	struct timeval tv;
	res = gettimeofday(&tv, NULL);
	t->sec = tv.tv_sec;
	t->usec = tv.tv_usec;
	return res;
}


/*int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t)
{
	return -1;
}*/


int os_gmtime(os_time_t t, struct os_tm *tm)
{
	struct tm *tm2;
	time_t t2 = t;

	tm2 = gmtime(&t2);
	if (tm2 == NULL)
		return -1;
	tm->sec = tm2->tm_sec;
	tm->min = tm2->tm_min;
	tm->hour = tm2->tm_hour;
	tm->day = tm2->tm_mday;
	tm->month = tm2->tm_mon + 1;
	tm->year = tm2->tm_year + 1900;
	return 0;
}


int os_get_random(unsigned char *buf, size_t len)
{
	return -1;
}


unsigned long os_random(void)
{
	return 0;
}


void * os_zalloc(size_t size)
{
	return calloc(1, size);
}


size_t os_strlcpy(char *dest, const char *src, size_t siz)
{
	const char *s = src;
	size_t left = siz;

	if (left) {
		/* Copy string up to the maximum size of the dest buffer */
		while (--left != 0) {
			if ((*dest++ = *s++) == '\0')
				break;
		}
	}

	if (left == 0) {
		/* Not enough room for the string; force NUL-termination */
		if (siz != 0)
			*dest = '\0';
		while (*s++)
			; /* determine total src string length */
	}

	return s - src - 1;
}


int os_memcmp_const(const void *a, const void *b, size_t len)
{
	const u8 *aa = a;
	const u8 *bb = b;
	size_t i;
	u8 res;

	for (res = 0, i = 0; i < len; i++)
		res |= aa[i] ^ bb[i];

	return res;
}


#ifdef OS_NO_C_LIB_DEFINES
void * os_malloc(size_t size)
{
	return NULL;
}


void * os_realloc(void *ptr, size_t size)
{
	return NULL;
}


void os_free(void *ptr)
{
}


void * os_memcpy(void *dest, const void *src, size_t n)
{
	return dest;
}


void * os_memmove(void *dest, const void *src, size_t n)
{
	return dest;
}


void * os_memset(void *s, int c, size_t n)
{
	return s;
}


int os_memcmp(const void *s1, const void *s2, size_t n)
{
	return 0;
}


char * os_strdup(const char *s)
{
	return NULL;
}


int os_strcasecmp(const char *s1, const char *s2)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strcmp(s1, s2);
}


int os_strncasecmp(const char *s1, const char *s2, size_t n)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strncmp(s1, s2, n);
}


char * os_strchr(const char *s, int c)
{
	return NULL;
}


char * os_strrchr(const char *s, int c)
{
	return NULL;
}


int os_strcmp(const char *s1, const char *s2)
{
	return 0;
}


int os_strncmp(const char *s1, const char *s2, size_t n)
{
	return 0;
}


char * os_strncpy(char *dest, const char *src, size_t n)
{
	return dest;
}


size_t os_strlcpy(char *dest, const char *src, size_t size)
{
	return 0;
}


char * os_strstr(const char *haystack, const char *needle)
{
	return NULL;
}


int os_snprintf(char *str, size_t size, const char *format, ...)
{
	return 0;
}
#endif /* OS_NO_C_LIB_DEFINES */
