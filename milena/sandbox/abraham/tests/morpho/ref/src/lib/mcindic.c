/* $Id: mcindic.c,v 1.4 2006/02/28 07:49:16 michel Exp $ */
/* gestion d'indicateurs binaires (jusqu'a 8) */
/* les indicateurs sont numerotes de 0 a 7 */
/* M. Couprie juillet 1996 */

/* gestion d'un indicateur binaire compact */
/* M. Couprie novembre 1999 */

/*
#define TESTINDIC
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcindic.h>

Indicstype *Indics = NULL;       /* en global pour etre efficace */

/* ==================================== */
void IndicsInit(int32_t Size)
/* ==================================== */
{
  int32_t i;
  Indics = (Indicstype *)calloc(Size, sizeof(Indicstype));
  if (Indics == NULL)
  {
    fprintf(stderr, "erreur allocation Indics\n");
    exit(1);
  }
}

/* ==================================== */
void Indics1bitInit(int32_t Size)
/* ==================================== */
{
  int32_t i;
  Indics = (Indicstype *)calloc((Size-1)/8 + 1, sizeof(Indicstype));
  if (Indics == NULL)
  {
    fprintf(stderr, "erreur allocation Indics\n");
    exit(1);
  }
}

/* ==================================== */
void Indics1bitReInit(int32_t Size)
/* ==================================== */
{
  memset(Indics, 0, ((Size-1)/8 + 1) * sizeof(Indicstype));
}

/* ==================================== */
void IndicsTermine()
/* ==================================== */
{
  free(Indics);
}

#ifdef TESTINDIC
void main()
{
  IndicsInit(3);
  Set(0, 0);   if (IsSet(0, 0)) printf("test1 ok\n");
printf("->%d\n", Indics[0]);
  Set(0, 1);   if (IsSet(0, 1)) printf("test2 ok\n");
printf("->%d\n", Indics[0]);
  UnSet(0, 1); if (!IsSet(0, 1)) printf("test3 ok\n");
printf("->%d\n", Indics[0]);
               if (IsSet(0, 0)) printf("test4 ok\n");
  UnSetAll(0); if (!IsSet(0, 0)) printf("test5 ok\n");
printf("->%d\n", Indics[0]);

  IndicsTermine();
}
#endif
