/***********************************************************************/
/*!
 * @file OFF.h
 * @date Mars. 2008
 * @author Julien Gony<BR>
 *         Université de Cergy-Pontoise<BR>
 *         ETIS - Equipe Image - UMR CNRS 8051
 *
 * @brief  Permet de lire, écrire obtenir le maillage et les points d'objet au format off
 *
 * Permet de lire, écrire obtenir le maillage et les points d'objet au format off
 *
 * @version 1.0
 */
/************************************************************************/

#ifndef __OFF_h__
#define __OFF_h__

#include <iostream>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief  Permet de lire, écrire obtenir le maillage et les points d'objet au format off
 *
 * Permet de lire, écrire obtenir le maillage et les points d'objet au format off
*/

class Off
{
	public:
		/**Constructeur par defaut*/
		Off():nbSommets(0),nbMailles(0),xSmt(0),ySmt(0),zSmt(0),mailles(0),isValid(true){};
		/**
		* @brief Constructeur.
		* Constructeur classique.
		* @param[in] xSmt vecteur des coords X
		* @param[in] ySmt vecteur des coords Y
		* @param[in] zSmt vecteur des coords Z
		* @param[in] mailles vecteur des mailles
		*/
		Off(const vector<float>& xSmt, const vector<float>& ySmt,const vector<float>& zSmt,const vector <int* >& mailles);
		
		/** Destructeur */
		~Off();
		
		/** Charger un off.
		* @param[in] nomFichier nom du fichier.
		*/
		bool charger(const string& nomFichier);
		
		/** Ecrire un off.
		 * @param[in] nomFichier nom du fichier.
		*/
		bool ecrire(const string& nomFichier);
		
		/** Verification d'un fichier off
		* @param[in] nomFichier nom du fichier
		*/
		static bool check(const string& nomFichier);
		
		//getters
		/** Retourne le vecteur des coords X*/
		const vector<float>& x()const{return xSmt;};
		/** Retourne le vecteur des coords Y*/
		const vector<float>& y()const{return ySmt;};
		/** Retourne le vecteur des coords Z*/
		const vector<float>& z()const{return zSmt;};
		/** Retourne le vecteur des mailles*/
		const vector<int*>& m()const{return mailles;};
		
	protected:

		int   nbSommets;/**< nombre de sommets*/
		int   nbMailles;/**< nombre de mailles*/
		
		vector<float> xSmt;/**< vecteur des coords X*/
		vector<float> ySmt;/**< vecteur des coords Y*/
		vector<float> zSmt;/**< vecteur des coords Z*/
		
		vector <int* > mailles;/**< vecteur des mailles*/
		
		bool isValid;/**< validité du off*/
};
#endif
