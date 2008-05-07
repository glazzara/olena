/* $Id: mcunionfind.h,v 1.3 2006/02/28 07:49:12 michel Exp $ */
/* ============== */
/* types publics  */
/* ============== */

typedef struct {
  int32_t Size;
  int32_t *Fth;
  int32_t *Rank;
} Tarjan;

/* ============== */
/* prototypes     */
/* ============== */

extern Tarjan * CreeTarjan(int32_t taille);
extern void TarjanTermine(Tarjan * T);
extern void TarjanInit(Tarjan * T);
extern void TarjanPrint(Tarjan * T);
extern void TarjanMakeSet(Tarjan * T, int32_t x);
extern int32_t TarjanFind(Tarjan * T, int32_t x);
extern int32_t TarjanLink(Tarjan * T, int32_t x, int32_t y);
extern int32_t TarjanLinkSafe(Tarjan * T, int32_t x, int32_t y);

