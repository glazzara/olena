#ifndef __calcul_h__
#define __calcul_h__

#include "WRL/Coin.h"
//#include "math.h"
#include <iostream>
#include <vector>

#define PRECISION 0.001
#define EPS 0.0000001

using namespace std;

//****************************************************************************
//		CALCUL SUR POINT
//****************************************************************************


point3D* produitVectoriel(const point3D *u, const point3D *v);
point3D* produitVectoriel(const point3D *u, const point3D *v, const point3D *ptCommun);
float norme(const point3D *u);
void normer(point3D *u);

float distance(const point3D *u, const point3D *v);

float angle(const point3D *u, const point3D *v);

//****************************************************************************
//		CALCUL SUR LISTE de POINTS
//****************************************************************************

Lpoints3D* produitVectoriel(const Lpoints3D *Lu, const Lpoints3D *Lv);
Lpoints3D* produitVectoriel(const Lpoints3D *Lu, const Lpoints3D *Lv, const Lpoints3D *LptCommun);
void normer(Lpoints3D **Lu);

#endif

