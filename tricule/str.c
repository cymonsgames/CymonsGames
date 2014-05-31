#include "vec.h"
#include "str.h"

void *ss_catcp(void *xx, int cp) {
	char *x = xx;
	int len, dummy=0;

	v_prune(x);
	len = v_count(x);

	v_atleast(x, v_count(x)+4);
	len += u8iwrite(x+len, &dummy, 4, cp);

	v_exact(x, len);
	v_adjoin(x, 0);

	return x;
}

void *ssw_catcp(void *xx, int cp) {
	wchar_t *x = xx;
	int len, dummy=0;

	v_prune(x);
	len = v_count(x);

	v_atleast(x, v_count(x)+2);
	len += u16iwrite(x+len, &dummy, 2, cp);

	v_exact(x, len);
	v_adjoin(x, 0);

	return x;
}

void *ss_catcu(void *xx, int cp) {
	char *x = xx;

	v_prune(x);
	v_adjoin(x, cp);
	v_adjoin(x, 0);

	return x;
}

void *ssw_catcu(void *xx, int cp) {
	wchar_t *x = xx;

	v_prune(x);
	v_adjoin(x, cp);
	v_adjoin(x, 0);

	return x;
}

void *ss_ncat(void *xx, const void *yy, int n) {
	char *x = xx;
	const char *y = yy;
	int len, i;

	for (i=0; i<n; ++i)
		if (!y[i])
			break;
	len = i;

	v_prune(x);
	v_append(x, y, len);
	v_adjoin(x, 0);

	return x;
}

void *ssw_ncat(void *xx, const void *yy, int n) {
	wchar_t *x = xx;
	const wchar_t *y = yy;
	int len, i;

	for (i=0; i<n; ++i)
		if (!y[i])
			break;
	len = i;

	v_prune(x);
	v_append(x, y, len);
	v_adjoin(x, 0);
	return x;
}

void *ss_cat(void *xx, const void *yy) {
	char *x = xx;
	const char *y = yy;
	int len = strlen(y);

	v_prune(x);
	v_append(x, y, len);
	v_adjoin(x, 0);

	return x;
}

void *ssw_cat(void *xx, const void *yy) {
	wchar_t *x = xx;
	const wchar_t *y = yy;
	int len = wcslen(y);

	v_prune(x);
	v_append(x, y, len);
	v_adjoin(x, 0);

	return x;
}

void *ss_cpy(void *xx, const void *yy) {
	char *x = xx;
	const char *y = yy;
	int len = strlen(y)+1;
	v_exact(x, len);
	strcpy(x, y);
	return x;
}

void *ssw_cpy(void *xx, const void *yy) {
	wchar_t *x = xx;
	const wchar_t *y = yy;
	int len = wcslen(y)+1;
	v_exact(x, len);
	wcscpy(x, y);
	return x;
}

void *ss_ncpy(void *xx, const void *yy, int n) {
	char *x = xx;
	const char *y = yy;

	int i;
	for (i=0; i<n; ++i)
		if (!y[i])
			break;
	n = i;

	v_exact(x, n+1);
	for (i=0; i<n; ++i)
		x[i] = y[i];
	x[i] = 0;
	return x;
}

void *ssw_ncpy(void *xx, const void *yy, int n) {
	wchar_t *x = xx;
	const wchar_t *y = yy;
	int i;
	for (i=0; i<n; ++i)
		if (!y[i])
			break;
	n = i;

	v_exact(x, n+1);
	for (i=0; i<n; ++i)
		x[i] = y[i];
	x[i] = 0;
	return x;
}

void *ss_trim(void *xx) {
	char *x = xx;
	char *y;
	char set[4] = " \n\t";
	int i, n, c, d;

	n = s_len(x);
	c = strspn (x, set);
	if (c)
		for (i = 0; i < n - c + 1; i++)
			x[i] = x[i + c];
	for (d = 1; d < n - c && !x[n - c - d + 1]; d++) {
		for (y = set; *y; y++)
			if (x[n - c - d] == *y)
				x[n - c - d] = 0;
	}
	n = strlen(x);
	v_exact(x, n + 1);

	return x;
}

void *ssw_trim(void *xx) {
	wchar_t *x = xx;
	wchar_t *y;
	wchar_t set[4] = L" \n\t";
	int i, n, c, d, onward = 1;

	n = s_len(x);
	onward = 1;
	for (c = 0; c < n && onward; c++) {
		onward = 0;
		for (y = set; *y; y++)
			if (x[c] == *y)
				onward = 1;
	}
	for (i = 0; i < n - c; i++)
		x[i] = x[i + c];
	onward = 1;
	for (d = 0; d < n - c && onward; d++) {
		onward = 0;
		for (y = set; *y; y++)
			if (x[n - c - d] == *y)
				onward = 1;
	}
	x[n - c - d] = 0;
	v_exact(x, n - c - d);

	return x;
}

