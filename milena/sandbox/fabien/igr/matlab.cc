#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/cast_image.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dicom/load.hh>

#include <mln/accu/count.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/sum.hh>
#include <mln/accu/image/all.hh>
#include <mln/arith/minus.hh>
#include <mln/arith/all.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/level/compute.hh>
#include <mln/math/abs.hh>
#include <mln/pw/all.hh>
#include <mln/trait/concrete.hh>
#include <mln/util/array.hh>

#include <cmath>

using namespace mln;


inline
image2d<float> mean_slices(util::array<image2d<float> >& arr_ima, int first, int last)
{
  image2d<accu::mean<float> > result;

  mln_precondition(first >=0 && first < arr_ima.nelements());
  mln_precondition(last >=0 && last < arr_ima.nelements());

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

  int acqui = 3;  // durée acqui et temps interimages; en secondes
  int fseuil = 4; // ratio entre signaux trop faibles, éliminés, et bruit de fond mesuré sur l image
  int ini = 9;    // nombre images ligne de base
  int ini2 = 20;  // a la fin de la montée vasculaire, à la dixieme image post injection: lissage des images par trois

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
  unsigned nbrpix1 = level::compute(accu_nbrpix1, roi_noise);
  image2d<float> datasli = arr_ima[0];

  image2d<float> prodsignal1;
  data::fill(prodsignal1, 0.0);
  data::paste(datasli | pw::value(roi_noise) == pw::cst(true), prodsignal1);

  // moyenne
  accu::mean<float> accu_mean;
  float moysignal1 = level::compute(accu_mean, prodsignal1 | pw::value(roi_noise) == pw::cst(true));
  // ecart type
  int som1 = 0;
  int kk = 0;
  mln_piter_(image2d<float>) p(datasli.domain());
  for_all(p)
    if (roi_noise(p))
      som1 += std::pow(math::abs(datasli(p) - moysignal1), 2);
  float ectys = std::sqrt(som1 / (nbrpix1 - 1));
  float seuil = fseuil * ectys;

  // Calcul du masque
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

  // Essai de lissage à parir de fin de montée vaculaire ini2 pour ameliorer
  // les seuillages a 10 50 90        NON EVALUE RIGOUREUSEMENT
  util::array<image2d<double> > arr_smooth;
  arr_smooth.append(arr_ima[0] * 1.0);
  for (int k = ini2 - 1; k < dim3 - 1; ++k)
    arr_smooth.append(arr_ima[k] * 0.5 + arr_ima[k - 1] * 0.25 + arr_ima[k + 1] * 0.25);
  arr_smooth.append(arr_ima[dim3 - 1] * 1.0);

  for_all(p)
    if (masque(p) == false)
    {
      for (int k = 0; k < arr_smooth.nelements(); ++k)
	arr_smooth[k](p) = 0.0;
      ima_c(p) = 0.0;
      ima_t(p) = 0.0;
      ++kk;
    }

  // FIXME: [c,T]=max(image,[ ],3);

  // Ou 'ima_c' est la valeur du max et 'ima_t' son index, le long de la dimension 3
  // kk est le nombre de points masques

  std::cout << "kk = " << kk << std::endl;

  image2d<accu::sum<float> > accu_sum;
  accu::image::init(result);
  for (int i = first; i < last; ++i)
    accu::image::take(result, arr_ima[i]);
  ima_auc accu::image::to_result(result);

  // Conversion du temps du pic en secondes à partir fin période de base
  tmax = acqui * (ima_t - ini);

  // calcul des temps 10 et 90 et de la pente correspondante
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
    {
      [C10(i,j),idx_10(i,j)]=max(image(i,j,:)>=0.1*c(i,j));
      [C90(i,j),idx_90(i,j)]=max(image(i,j,:)>=0.9*c(i,j));
      [C50(i,j),idx_50(i,j)]=max(image(i,j,:)>=0.5*c(i,j));
      if (idx_90(i,j)-idx_10(i,j)==0)
	masque2(i,j)=0;
      else
	masque2(i,j)=1;
    }

  // calcul MTT

  // QUAND le signal redescent significativement après le maximum
  // elimination des voxels où le signal est encore 90//  du maximum en fin
  // d'acquisition
  int kk2 = 0;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      if (image(i,j,dim3)>=0.9*c(i,j))
      {
	masque3(i,j)=0;
	kk2=kk2+1;
      }
      else
	masque3(i,j)=1;

  // pour les voxels non masqués on inverse t et on cherche le max 50//  a partir de la fin
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      for (int k = 0; k < dim3; ++k)
	imageinv(i,j,k)=masque3(i,j)*image(i,j,dim3+1-k);

  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      if (masque3(i,j)==0)
      {
	idx_51(i,j)=0;
	C51(i,j)=0;
      }
      else
	[C51(i,j),idx_51(i,j)]=max(imageinv(i,j,:)>=0.8*c(i,j));

  std::cout << "kk2 = " << kk2 << std::endl;

  // Calcul MTT  defini comme largeur a mi hauteur de la courbe signal(temps)
  // soit t50 montant et t50 descendant
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      if (masque(i,j)*masque2(i,j)*masque3(i,j)==0)
	MTT(i,j)=0;
      else
	MTT(i,j)=acqui*(dim3-idx_51(i,j)-idx_50(i,j));

  // calcul de la pente 10/90 (avec delta signal=80// du maximum c(i,j) et
  // delta temps =t90 -t10
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      if (masque(i,j)*masque2(i,j)==0)
	pente(i,j)=0;
      else
	pente(i,j)=(0.8*c(i,j))/(acqui*(idx_90(i,j)-idx_10(i,j)));

  // calcul pente normalisée rehaussement normalisé et AUC normalisée
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      if (masque(i,j)==0)
      {
	pentenorm(i,j)=0;
	reh(i,j)=0;
	AUCnorm(i,j)=0;
      }
      else
      {
	pentenorm(i,j)=pente(i,j)/imageini(i,j);
	reh(i,j)=c(i,j)/imageini(i,j);
	AUCnorm(i,j)=AUC(i,j)/imageini(i,j);
      }

  /*figure (3)
    imagesc(AUCnorm);
  title('image aire normalisée sous la courbe');
  //  l'aire sous la courbe est négative là où il y a eu du bougé (bord tube)


  figure (4)
    imagesc(pentenorm);
  title('pente normalisée10-90');

  figure(5)
    imagesc(reh);
  title('image rehaussement relatif');
  caxis([0 5]);
  // au dela de 5 il faut réviser la dose d'agent de contraste car surdosage

  figure(6)
    imagesc(MTT);
  title('image MTT');

  figure(9)
    imagesc(idx_51);
  title('idx_51');

  figure(10)
    imagesc(C51);

  figure(7)
    imagesc(tmax);
  title('image Tmaximum');

  figure(8)
    subplot(2,2,1);
  imagesc(pentenorm);
  title('pente normalisée10-90');
  subplot(2,2,2)
    imagesc(reh);
  title('image rehaussement relatif');
  caxis([0 3]);
  subplot(2,2,3)
    imagesc(tmax);
  title('image Tmaximum');*/

  //  COURBE S(t)sur pixel choisi graphiquement
  // Permet de voir la courbe de rehaussement d'un voxel pointé
  // SI LE VOXEL EST AU BORD DU TUBE TEMOIN: UN DETECTEUR DE MOUVEMENTS
  //  coordonx=1:size(image,3);
  x = size(image, 1) / 2;
  y = size(image, 2) / 2;
  while (x < size(image, 1) && y < size(image, 2))
  {
    //  la saisie du point de coordonnées x,y se fait sur la figure ouverte en
    //  dernier
    //  pour sortir: cliquer dans le gris...
    //  pour affichage interactif coordonnées pixel, mais en fait pas utile
    h=figure(8)
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
    fprintf(1,'\n// d\t// d\t// f\t// f\n',coordx,coordy,reh(coordx,coordy),image(coordx,coordy,1));
    titi = (abs(image(coordx, coordy, :)));
    tata = squeeze(titi);
    subplot(2,2,4)
      plot(tata);
  }

  return 0;
}
