#define SSW(x,fn) ((sizeof*(x)==1)? ss_##fn: ssw_##fn)

#if !defined __cplusplus
#define s_cpy(x,y) ((x) = SSW(x,cpy)(x,y))
#define s_cat(x,y) ((x) = SSW(x,cat)(x,y))
#define s_ncat(x,y,n) ((x) = SSW(x,ncat)(x,y,n))
#define s_ncpy(x,y,n) ((x) = SSW(x,ncpy)(x,y,n))
#define s_catcp(x,cp) ((x) = SSW(x,catcp)(x,cp))
#define s_adjoin(x,cu) ((x) = SSW(x,catcu)(x,cu))
#define s_trim(x) ((x) = SSW(x,trim)(x))
#define s_free(x) v_free(x)
#else
#define s_cpy(x,y) (*(void **)&(x)=SSW(x,cpy)(x,y))
#define s_cat(x,y) (*(void **)&(x)=SSW(x,cat)(x,y))
#define s_ncat(x,y,n) (*(void **)&(x)=SSW(x,ncat)(x,y,n))
#define s_ncpy(x,y,n) (*(void **)&(x)=SSW(x,ncpy,void *(*)())(x,y,n))
#define s_catcp(x, cp) (*(void **)&(x)=SSW(x,catcp)(x,cp))
#define s_free(x) v_free((void*)x)
#define s_trim(x) (*(void **)&(x)=SSW(x,trim)(x))

#endif

	/* you can use strlen also... this is somewhat maybe more efficient.
 	   would definitely be more efficient if we used VEC(x)->nelt-1 to
	   bypass the check for NULL pointer. */
#define s_len(x) (v_count(x)-1)
#define s_chop(x) (v_chop(x),v_adjoin(x,0))
#define s_last(x) (s_len(x)>1?(x)[s_len(x)-1]:0)

#if 0
#define s_fmt(x,fmt,...) 

/* Unfortunately, there is not a way to leverage the standard library's
   printf here because we don't have a way to predetermine the output size. */
char *ss_fmt(char *x, const char *fmt, ...);
#endif

void *ss_ncpy(void *x, const void *y, int n) ;
void *ssw_ncpy(void *x, const void *y, int n) ;

/* gcc insists on complaining about these. its diagnostic message even
   says it inserts code to abort instead of to perform the call as requested.
   for this reason the parameters here have been relaxed to void *'s */
void *ss_cat(void *x, const void *y) ;
void *ssw_cat(void *x, const void *y) ;
void *ss_ncat(void *x, const void *y, int n) ;
void *ssw_ncat(void *x, const void *y, int n) ;
void *ss_cpy(void *x, const void *y) ;
void *ssw_cpy(void *x, const void *y) ;
void *ss_catcp(void *x, int cp) ;
void *ssw_catcp(void *x, int cp) ;
void *ss_trim(void *xx);
void *ssw_trim(void *xx);

void *ss_catcu(void *x, int cu) ;
void *ssw_catcu(void *x, int cu) ;
