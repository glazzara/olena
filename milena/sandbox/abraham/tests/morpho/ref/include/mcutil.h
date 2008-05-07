/* $Id: mcutil.h,v 1.7 2006/02/28 07:49:12 michel Exp $ */
#define mcabs(X) ((X)>=0?(X):-(X))
#define max(X,Y) ((X)>=(Y)?(X):(Y))
#define min(X,Y) ((X)<=(Y)?(X):(Y))
#define odd(X) ((X)&1)
#define even(X) (((X)&1)==0)
#define arrondi(z) (((z)-(double)((int32_t)(z)))<=0.5?((int32_t)(z)):((int32_t)(z+1)))
#define signe(z) (((z)>0.0)?1.0:-1.0)
#define sqr(x) ((x)*(x))

/* retourne VRAI si Z est plus proche de A que de B */
#define PlusProche(Z,A,B) (abs((Z)-(A))<abs((Z)-(B)))

#define TestNonNul(P) {if((P)==NULL){\
perror("Erreur fatale : memoire insuffisante (malloc)\n");exit(0);}}

#ifndef M_PI
# define M_E		2.7182818284590452354	/* e */
# define M_LOG2E	1.4426950408889634074	/* log_2 e */
# define M_LOG10E	0.43429448190325182765	/* log_10 e */
# define M_LN2		0.69314718055994530942	/* log_e 2 */
# define M_LN10		2.30258509299404568402	/* log_e 10 */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.78539816339744830962	/* pi/4 */
# define M_1_PI		0.31830988618379067154	/* 1/pi */
# define M_2_PI		0.63661977236758134308	/* 2/pi */
# define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
# define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
# define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */
#endif
