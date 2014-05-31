#include "vec.h"

#if !defined NDEBUG && defined _CRTDBG_MAP_ALLOC
	#define vmalloc(x) _malloc_dbg(x, _NORMAL_BLOCK, fname, line)
	#define vrealloc(x,y) _realloc_dbg(x,y, _NORMAL_BLOCK, fname, line)
#else
	#define vmalloc(x) malloc(x)
	#define vrealloc(x,y) realloc(x,y)
#endif

void *v_alloc(int size V_DEBUGPARAMS) {
	struct vec *v;
	v = vmalloc(sizeof *v);
	v->melt = 0;
	v->nelt = 0;
	v->size = size;
	v->csum = 0;
	return v+1;
}

void v_free(void *x) {
	if (x)
		free(VEC(x));
}

void *vv_growone(void *x, int size V_DEBUGPARAMS) {
	struct vec *v = VEC(x);

	if (!x) {
		v = vmalloc(sizeof *v + size);
		v->melt = 1;
		v->nelt = 1;
		v->size = size;
		v->csum = 0;
		return v+1;
	}

	assert(v->size == size);

	if (v->nelt++ == v->melt) {
		v->melt = 2 * v->melt + 1;
		v = vrealloc(v, sizeof *v + v->melt * size);
		return v + 1;
	}

	return x;
}

void *vv_growk(void *x, int size, int k V_DEBUGPARAMS) {
	struct vec *v = VEC(x);

	if (!x) {
		v = vmalloc(sizeof *v + k * size);
		v->melt = k;
		v->nelt = k;
		v->size = size;
		v->csum = 0;
		return v+1;
	}

	assert(v->size == size);

	if (v->melt < (v->nelt += k)) {
		v->melt = 2 * (v->nelt);
		v = vrealloc(v, sizeof *v + v->melt * size);
		return v + 1;
	}

	return x;
}

void *vv_atleast(void *x, int size, int m V_DEBUGPARAMS) {
	struct vec *v = VEC(x);

	if (!x) {
		v = vmalloc(sizeof *v + m * size);
		v->melt = m;
		v->nelt = 0;
		v->size = size;
		v->csum = 0;
		return v+1;
	}

	assert(v->size == size);

	if (v->melt < m) {
		v->melt = v->melt + m;
		v = vrealloc(v, sizeof *v + v->melt * size);
		return v + 1;
	}

	return x;
}

void vv_remove(void *xx, int size, int i, int n) {
	char *x = xx;
	assert(size == VEC(x)->size);

	memmove(x+i*size, x+(i+n)*size, size * (VEC(x)->nelt-i-n));
	VEC(x)->nelt -= n;
}

void *vv_insert(void *xx, int size, int i, void *e, int n V_DEBUGPARAMS) {
	char *x = xx;
	assert(!x || size == VEC(x)->size);

	x=vv_growk(x, size, n V_DEBUGARGS);
	memmove(x+(i+n)*size, x+i*size, size*(VEC(x)->nelt-n-i));
	memcpy(x+i*size, e, n*size);
	return x;
}

void *vv_concat(void *x, int size, void *y V_DEBUGPARAMS) {
	if (y) {
		assert(!x || size == VEC(x)->size);
		assert(size == VEC(y)->size);
		vv_insert(x, size, VEC(x)->nelt, y, VEC(y)->nelt V_DEBUGARGS);
	}

	return x;
}

