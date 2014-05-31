#include "defines.h"

#if !defined NDEBUG && defined _CRTDBG_MAP_ALLOC
	#define V_DEBUGARGS , __FILE__ , __LINE__
	#define V_DEBUGPARAMS , const char *fname, int line
	#define V_DEBUGCARRY  , fname, line
#else
	#define V_DEBUGARGS 
	#define V_DEBUGPARAMS 
	#define V_DEBUGCARRY
#endif

DEFINE_STRUCT_VEC
#undef DEFINE_STRUCT_VEC
#define DEFINE_STRUCT_VEC

#define VEC(x)  ((struct vec *)((char *)(x)-sizeof (struct vec)))

void *v_alloc(int size V_DEBUGPARAMS) ;
void v_free(void *x) ;
#if defined __cplusplus
	/* C++ has a nightmare of a type system to work around. */
#define v_adjoin(x,e) \
    ((*(void **)&(x))=vv_growone((x),sizeof*(x) V_DEBUGARGS), \
     (x)[VEC(x)->nelt-1]=(e))

#else
#define v_adjoin(x,e) \
    ((x)=vv_growone((x),sizeof*(x) V_DEBUGARGS), (x)[VEC(x)->nelt-1]=(e))
#define v_grow(x,k) \
    (v_count(x)<(k)?(x)=vv_growk(x,sizeof*(x),(k)-(v_count(x)) V_DEBUGARGS):0)

#endif

#define v_growone(x) ((x)=vv_growone((x),sizeof*(x) V_DEBUGARGS))

#define v_append(x,e,k) \
    ((x)=vv_growk((x),sizeof*(x),k V_DEBUGARGS),  \
     memcpy((x)+VEC(x)->nelt-(k), e, (k)*sizeof *(x)))

#define v_remove(x,i,n) vv_remove(x, sizeof *(x), i, n)
#define v_insert(x,i,e,n) ((x)=vv_insert(x, sizeof *(x), i, e, n V_DEBUGARGS))
#define v_concat(x,y) ((x)=vv_concat(x, sizeof *(x), y V_DEBUGARGS))
#define v_count(x) ((x)?VEC(x)->nelt:0)
#define v_avail(x) ((x)?VEC(x)->melt:0)

/* increases the capacity of the buffer if needed (does not affect nelt) */
#define v_atleast(x,m) \
	(v_avail(x)<(m)?(x)=vv_atleast(x,sizeof*(x),(m) V_DEBUGARGS):0)

#define v_exact(x,n) (v_atleast(x,n), (x)?VEC(x)->nelt = (n):0)
#define v_chop(x) ((x)?VEC(x)->nelt=0:0)
#define v_prune(x) ((x)?(x)[--VEC(x)->nelt]:0)
#define v_cutlast(x) ((x)[--VEC(x)->nelt])
#define v_last(x) ((x)[VEC(x)->nelt-1])

void *vv_growone(void *x, int size                       V_DEBUGPARAMS) ;
void *vv_growk(void *x, int size, int k                  V_DEBUGPARAMS) ;
void *vv_atleast(void *x, int size, int k                V_DEBUGPARAMS) ;
void vv_remove(void *x, int size, int i, int n) ;
void *vv_insert(void *x, int size, int i, void *e, int n V_DEBUGPARAMS) ;
void *vv_concat(void *x, int size, void *y               V_DEBUGPARAMS) ;

/* Demonstration use:
 *   int *q = 0;
 *   v_append(q, 4);
 *   v_append(q, 6);
 *   v_append(q, 5);
 *   ++q[1];
 *   qsort(q, v_count(q), sizeof *q, cmpint);
 *   v_free(q);
 *
 * If inserts are going to be called (that could trigger a resize), then
 * q must be passed by reference. That is, the v_append, v_insert, and
 * v_concat routines can cause q to change its value.
 *
 * Client code is supposed to use these v_ macros, but vv_ versions are
 * available as well.
 */

