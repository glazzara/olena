/***********************************************************************/
/*!
 * @file Objet3D.h
 * @date Mars. 2008
 * @author Julien Gony<BR>
 *         Université de Cergy-Pontoise<BR>
 *         ETIS - Equipe Image - UMR CNRS 8051
 *
 * @brief  Permet de lire, écrire, obtenir le maillage et les points d'un objet 3D
 *
 * Permet de lire, écrire, obtenir le maillage et les points d'un objet 3D
 *
 * @version 1.0
 */
/************************************************************************/

#ifndef __Objet3D_h__
#define __Objet3D_h__

#include "WRL/Coin.h"
#include <iostream>

using std::string;

/**
 * @brief  Permet de lire, écrire, obtenir le maillage et les points d'un objet 3D
 *
 * Permet de lire, écrire, obtenir le maillage et les points d'un objet 3D
 */
class Objet3D
{
	public:
		enum format {INVALID=0, VRML=1, OFF=2};
		
		/**
		 * @brief Constructeur depuis un fichier.
		 * Construit un objet 3D à partir d'un fichier.
		 * @param[in] nomFichier nom du fichier.
		*/
		Objet3D(const string& nomFichier);
		
		/**
		 * @brief Constructeur avec une liste de points et une liste de mailles.
		 * Construit un objet 3D à partir d'une liste de points et d'une liste de mailles.
		 * @param[in] points liste de points.
		 * @param[in] type format de l'objet(off,vrml,...).
		*/
		Objet3D(Lpoints3D *points=NULL, Lmailles *mailles=NULL,int type=INVALID);
		
		/** Destructeur */
		~Objet3D();
		
		/**
		 * @brief Verification de la validité d'un fichier.
		 * Verifie la validité d'un fichier objet 3D.
		 * @param[in] nomFichier nom du fichier.
		 */		
		static bool check(const string& nomFichier);
		
		/**
		 * @brief Trouve le type d'un fichier.
		 * Renvoie le type d'un fichier objet3D (VRML,OFF,...).
		 * @param[in] nomFichier nom du fichier.
		*/
		static int type(const string& fichier);
		
		//getters
		/** Retourne le type de l'objet*/
		int type()const{return _type;};
		/** Retourne le nom du fichier de l'objet*/
		const string& nomFichier()const{return _nomFichier;};
		/** Retourne la liste des points*/
		Lpoints3D* points()const{return _points;};
		/** Retourne la liste des mailles*/
		Lmailles* mailles()const{return _mailles;};
		
		//setters
		/** Affecte un type
		 * @param[in] tp type
		*/
		void type(int tp){_type = tp;};
		
	protected:
		int _type;/**<type d'objet*/
		string _nomFichier;/**<nom du fihier*/
		Lpoints3D* _points;/**< liste des points*/
		Lmailles* _mailles;/**< liste des mailles*/
};
#endif
