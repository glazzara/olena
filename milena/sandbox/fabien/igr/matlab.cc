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
#include <mln/arith/plus.hh>
#include <mln/arith/times.hh>
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
  {
    if (roi_noise(p))
      som1 += std::pow(math::abs(datasli(p) - moysignal1), 2);
  }
  float ectys = std::sqrt(som1 / (nbrpix1 - 1));
  float seuil = fseuil * ectys;

  // Calcul du masque
  image2d<bool> masque;
  initialize(masque, datasli);
  data::fill(masque, false);
  data::fill(masque | pw::value(datasli) > pw::cst(seuil), true);
  // si on a choisi une région avec roi2
  if (nargin>2)
    data::fill(masque | pw::value(roi2) == false, false);

  // On applique le masque sur image et imasoustraite
  for (int k = 0; k < arr_ima.nelements(); ++k)
  {
    data::fill(arr_ima[k] | pw::value(masque) == false, 0.0);
    data::fill(arr_sous[k] | pw::value(masque) == false, 0.0);
  }

  // On regarde si le seuillage et le masquage sont OK
  // FIXME: imagesc(abs(image(:,:,2)));

  // On cherche les maxi (intensité c et temps T) des courbes signal(temps)
  // On masque toute la série d'images

  // Essai de lissage à parir de fin de montée vaculaire ini2 pour ameliorer
  // les seuillages a 10 50 90        NON EVALUE RIGOUREUSEMENT
  for (int k = init2 - 1; k < dim3 - 1; ++k)
    arr_ima[k] = 0.5 * arr_ima[k] + 0.25 * arr_ima[k - 1] + 0.25 * arr_ima[k + 1];

  for_all(p)
  {
    if (masque(p) == false)
    {
      for (int k = 0; k < arr_ima.nelements(); ++k)
	arr_ima[k](p) = 0.0;
      ima_c(p) = 0.0;
      ima_t(p) = 0.0;
      ++kk;
    }
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

  return 0;
}
