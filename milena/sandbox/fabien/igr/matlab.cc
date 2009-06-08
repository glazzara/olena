#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dicom/load.hh>

#include <mln/accu/count.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/sum.hh>
#include <mln/accu/image/all.hh>
#include <mln/arith/minus.hh>
#include <mln/arith/all.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/data/compute.hh>
#include <mln/math/abs.hh>
#include <mln/pw/all.hh>
#include <mln/trait/concrete.hh>
#include <mln/util/array.hh>

#include <cmath>

using namespace mln;


inline
image2d<float> mean_slices(util::array<image2d<float> >& arr_ima, int first, int last)
{
  image2d<accu::stat::mean<float> > result;

  mln_precondition(first >=0 && first < arr_ima.nelements());
  mln_precondition(last >=0 && last < arr_ima.nelements());

  initialize(result, arr_ima[first]);

  accu::image::init(result);
  for (int i = first; i < last; ++i)
    accu::image::take(result, arr_ima[i]);

  return accu::image::to_result(result);
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << "input" << std::endl;
    return 1;
  }

  image3d<value::int_u12> input;
  io::dicom::load(input, argv[1]);
  util::array<image2d<float> > arr_ima;
  for (int i = 0; i < input.nslices(); ++i)
    arr_ima.append(duplicate(cast_image<float>(slice(input, i))));

  const int acqui = 3;  // durée acqui et temps interimages; en secondes
  const int fseuil = 4; // ratio entre signaux trop faibles, éliminés, et bruit de fond mesuré sur l image
  const int ini = 9;    // nombre images ligne de base
  const int ini2 = 20;  // a la fin de la montée vasculaire, à la dixieme image post injection: lissage des images par trois

  // Calcul signal initialmoyen : 8 images de 2 à 9 moyennées=ligne de base
  image2d<float> imageini = mean_slices(arr_ima, 1, 8);

  int dim1 = imageini.nrows();
  int dim2 = imageini.ncols();
  int dim3 = arr_ima.nelements();

  // Calcul auc aire sous la courbe
  util::array<image2d<float> > arr_sous;
  for (int k = 0; k < arr_ima.nelements(); ++k)
    arr_sous.append(arr_ima[k] - imageini);

  // Mesure bruit de fond pour seuiller
  // calculé sur la première image
  image2d<bool> roi_noise; // FIXME: init this ROI, should be a domain
  accu::count<bool> accu_nbrpix1;
  unsigned nbrpix1 = data::compute(accu_nbrpix1, roi_noise);
  image2d<float> datasli = arr_ima[0];

  image2d<float> prodsignal1;
  data::fill(prodsignal1, 0.0);
  data::paste(datasli | pw::value(roi_noise) == pw::cst(true), prodsignal1);

  /////////////
  //	     //
  // Moyenne //
  //	     //
  /////////////
  accu::stat::mean<float> accu_mean;
  float moysignal1 = data::compute(accu_mean, prodsignal1 | pw::value(roi_noise) == pw::cst(true));

  ////////////////
  //		//
  // Ecart type //
  //		//
  ////////////////
  int som1 = 0;
  int kk = 0;
  mln_piter_(image2d<float>) p(datasli.domain());
  for_all(p)
    if (roi_noise(p))
      som1 += std::pow(math::abs(datasli(p) - moysignal1), 2);
  float ectys = std::sqrt(som1 / (nbrpix1 - 1));
  float seuil = fseuil * ectys;

  //////////////////////
  //		      //
  // Calcul du masque //
  //		      //
  //////////////////////
  image2d<bool> masque;
  initialize(masque, datasli);
  data::fill(masque, false);
  data::fill((masque | pw::value(datasli) > pw::cst(seuil)).rw(), true);

  // si on a choisi une région avec roi
  int nargin = 0;
  image2d<bool> roi; // FIXME: init this ROI, should be a domain
  if (nargin > 2)
    data::fill((masque | pw::value(roi) == false).rw(), false);

  // On applique le masque sur image et imasoustraite
  for (int k = 0; k < arr_ima.nelements(); ++k)
  {
    data::fill((arr_ima[k] | pw::value(masque) == false).rw(), 0.0);
    data::fill((arr_sous[k] | pw::value(masque) == false).rw(), 0.0);
  }

  // On regarde si le seuillage et le masquage sont OK
  // FIXME: imagesc(abs(image(:,:,2)));

  // On cherche les maxi (intensité ima_c et temps ima_t) des courbes signal(temps)
  // On masque toute la série d'images

  /////////////
  //	     //
  // Lissage //
  //	     //
  /////////////

  // Essai de lissage à parir de fin de montée vaculaire ini2 pour ameliorer
  // les seuillages a 10 50 90        NON EVALUE RIGOUREUSEMENT
  util::array<image2d<double> > arr_smooth;
  arr_smooth.append(arr_ima[0] * 1.0);
  for (int k = ini2 - 1; k < dim3 - 1; ++k)
    arr_smooth.append(arr_ima[k] * 0.5 + arr_ima[k - 1] * 0.25 + arr_ima[k + 1] * 0.25);
  arr_smooth.append(arr_ima[dim3 - 1] * 1.0);

  ///////////////////////////////////
  //				   //
  // Calcul image max et temps max //
  //				   //
  ///////////////////////////////////
  image2d<float> ima_c;
  initialize(ima_c, arr_smooth[0]);
  data::fill(ima_c, 0.0);
  image2d<unsigned> ima_t;
  initialize(ima_t, ima_c);
  for_all(p)
    if (masque(p) == false)
    {
      for (int k = 0; k < arr_smooth.nelements(); ++k)
	arr_smooth[k](p) = 0.0;
      ima_c(p) = 0.0;
      ima_t(p) = 0;
      ++kk;
    }

  for_all(p)
    for (unsigned k = 0; k < dim3; ++k)
      if (ima_c(p) < arr_smooth[k](p))
      {
	ima_c(p) = arr_smooth[k](p);
	ima_t(p) = k;
      }

  // Ou 'ima_c' est la valeur du max et 'ima_t' son index, le long de la dimension 3
  // kk est le nombre de points masques

  std::cout << "kk = " << kk << std::endl;

  image2d<accu::sum<float> > accu_sum;
  accu::image::init(accu_sum);
  for (int k = 0; k < dim3; ++k)
    accu::image::take(accu_sum, arr_smooth[k]);
  image2d<float> ima_auc = accu::image::to_result(accu_sum);

  // Conversion du temps du pic en secondes à partir fin période de base
  image2d<int> tmax = acqui * (ima_t - ini);

  // calcul des temps 10 et 90 et de la pente correspondante
  image2d<float> ima_c10;
  initialize(ima_c10, arr_smooth[0]);
  data::fill(ima_c10, 0.0);
  image2d<unsigned> ima_idx10;
  initialize(ima_idx10, ima_c10);
  bool set_10 = false;

  image2d<float> ima_c90;
  initialize(ima_c90, arr_smooth[0]);
  data::fill(ima_c90, 0.0);
  image2d<unsigned> ima_idx90;
  initialize(ima_idx90, ima_c90);
  bool set_90 = false;

  image2d<float> ima_c50;
  initialize(ima_c50, arr_smooth[0]);
  data::fill(ima_c50, 0.0);
  image2d<unsigned> ima_idx50;
  initialize(ima_idx50, ima_c50);
  bool set_50 = false;

  image2d<bool> masque2;
  initialize(masque2, arr_smooth[0]);

  // FIXME: This could be done with a bkw_piter.
  for_all(p)
  {
    for (unsigned k = 0; k < dim3; ++k)
    {
      if (!set_10 && arr_smooth[k](p) >= 0.1 * ima_c(p))
      {
	ima_c10(p) = arr_smooth[k](p);
	ima_idx10(p) = k;
	set_10 = true;
      }
      if (!set_90 && arr_smooth[k](p) >= 0.9 * ima_c(p))
      {
	ima_c90(p) = arr_smooth[k](p);
	ima_idx90(p) = k;
	set_90 = true;
      }
      if (!set_50 && arr_smooth[k](p) >= 0.5 * ima_c(p))
      {
	ima_c50(p) = arr_smooth[k](p);
	ima_idx50(p) = k;
	set_50 = true;
      }
    }
    masque2(p) = (ima_idx90(p) != ima_idx10(p));
  }

  ////////////////
  //		//
  // Calcul MTT //
  //		//
  ////////////////

  // QUAND le signal redescent significativement après le maximum
  // elimination des voxels où le signal est encore 90//  du maximum en fin
  // d'acquisition
  image2d<bool> masque3;
  initialize(masque3, masque2);
  int kk2 = 0;
  for_all(p)
    if (arr_smooth[dim3 - 1](p) >= 0.9 * ima_c(p))
    {
      masque3(p) = false;
      ++kk2;
    }
    else
      masque3(p) = true;

  // pour les voxels non masqués on inverse t et on cherche le max 50//  a partir de la fin
  util::array<image2d<float> > arr_inv;
  for (int k = 0; k < dim3; ++k)
  {
    arr_inv.append(arr_ima[dim3 - 1 - k]);
    data::fill((arr_inv[k] | pw::value(masque3) == false).rw(), 0.0);
  }

  image2d<float> ima_c51;
  initialize(ima_c51, arr_smooth[0]);
  data::fill(ima_c51, 0.0);
  image2d<unsigned> ima_idx51;
  initialize(ima_idx51, ima_c51);
  bool set_51 = false;
  for_all(p)
  {
    if (masque3(p))
    {
      for (unsigned k = 0; k < dim3; ++k)
	if (!set_50 && arr_smooth[k](p) >= 0.8 * ima_c(p))
	{
	  ima_c51(p) = arr_inv[k](p);
	  ima_idx51(p) = k;
	  set_51 = true;
	}
    }
    else
    {
      ima_c51(p) = 0;
      ima_idx51(p) = 0;
    }
  }

  std::cout << "kk2 = " << kk2 << std::endl;

  // Calcul MTT  defini comme largeur a mi hauteur de la courbe signal(temps)
  // soit t50 montant et t50 descendant
  image2d<int> ima_mtt;
  initialize(ima_mtt, masque);
  data::fill(ima_mtt, 0);
  for_all(p)
    if (masque(p) && masque2(p) && masque3(p))
      ima_mtt(p) = acqui * (dim3 - ima_idx51(p) - ima_idx50(p));

  // calcul de la pente 10/90 (avec delta signal=80// du maximum c(i,j) et
  // delta temps = t90 - t10
  image2d<float> ima_pente;
  initialize(ima_pente, masque);
  data::fill(ima_pente, 0);
  for_all(p)
    if (masque(p) && masque2(p))
      ima_pente(p) = (0.8 * ima_c(p)) / (acqui * (ima_idx90(p) - ima_idx10(p)));

  ////////////////////////////////////////////////
  //						//
  // Normalisation de la pente, du rehaussement //
  // et de l'AUC				//
  //						//
  ////////////////////////////////////////////////
  image2d<float> ima_pentenorm;
  initialize(ima_pentenorm, masque);
  data::fill(ima_pentenorm, 0);

  image2d<float> ima_reh;
  initialize(ima_reh, masque);
  data::fill(ima_reh, 0);

  image2d<float> ima_aucnorm;
  initialize(ima_aucnorm, masque);
  data::fill(ima_aucnorm, 0);

  for_all(p)
    if (masque(p))
    {
      ima_pentenorm(p) = ima_pente(p) / imageini(p);
      ima_reh(p) = ima_c(p) / imageini(p);
      ima_aucnorm(p) = ima_auc(p) / imageini(p);
    }

  //  COURBE S(t)sur pixel choisi graphiquement
  // Permet de voir la courbe de rehaussement d'un voxel pointé
  // SI LE VOXEL EST AU BORD DU TUBE TEMOIN: UN DETECTEUR DE MOUVEMENTS
  //  coordonx=1:size(image,3);
  float x = masque.nrows() / 2;
  float y = masque.ncols() / 2;
  /*while (x < masque.nrows() && y < masque.ncols())
  {
    //  la saisie du point de coordonnées x,y se fait sur la figure ouverte en
    //  dernier
    //  pour sortir: cliquer dans le gris...
    //  pour affichage interactif coordonnées pixel, mais en fait pas utile
    h=figure(8);
    pixval;
    // saisie coordonnées du pixel pour lequel on veut afficher la courbe de
    // décroissance et le fit
    [x,y]=ginput(1);
    //  on ajuste a l'entier supérieur pour trouver les "vraies" coordonnées
    //  en plus il faut inverser entre xy et ij...
    coordx=ceil(y);
    coordy=ceil(x);
    //  test sortie du programme
    if ((coordx > size(image, 1)) | (coordy > size(image, 2)))
      break;
    fprintf(1,"\n// d\t// d\t// f\t// f\n",coordx,coordy,reh(coordx,coordy),image(coordx,coordy,1));
    titi = (abs(image(coordx, coordy, :)));
    tata = squeeze(titi);
    subplot(2,2,4);
    plot(tata);
  }*/

  return 0;
}
