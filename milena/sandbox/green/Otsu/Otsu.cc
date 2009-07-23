// Méthode Otsu de binarisation
// Seuillage par minimisation des variances intra-classe

// Pour les flux d'entrées/sorties
#include <iostream>

// Pour les traces dans le code et le debuggage
#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

// Pour les opérateurs statistiques sur une image
#include <mln/accu/stat/min.hh>
#include <mln/accu/stat/max.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>
#include <mln/data/compute.hh>

// Pour les chargements/sauvegardes des images
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

// Pour les types de base
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

// Pour le seuillage d'une image
#include <mln/binarization/threshold.hh>

// Pour le mécanisme de restriction de domaine
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

// Pour le copier/coller et le remplissage
#include <mln/data/paste.hh>
#include <mln/data/fill.hh>

// Pour les macros basiques (affectation d'une image ...)
#include <mln/core/var.hh>

// Pour la négation du mask
#include <mln/logical/not.hh>



#define MAX_THRESHOLD 255


int main()
{
  using namespace mln;
  using namespace std;

//  trace::quiet = false;
  trace::entering("main");


// (1) Charger une image
  image2d<value::int_u8> input;
  io::pgm::load(input, "/usr/local/share/olena/images/small.pgm");


// (2) Etablir les caractéristiques globales de l'image (min, max, mean, variance)
  float         count = data::compute(accu::meta::math::count(), input);
  value::int_u8 min   = data::compute(accu::meta::stat::min(), input);
  value::int_u8 max   = data::compute(accu::meta::stat::max(), input);
  float         mean  = data::compute(accu::meta::stat::mean(), input);
  float         var   = data::compute(accu::stat::variance<value::int_u8>(), input);

  cout << "CARACTERISTIQUES DE L'IMAGE" << endl;
  cout << "COUNT = " << count << endl;
  cout << "MIN   = " << min   << endl;
  cout << "MAX   = " << max   << endl;
  cout << "MEAN  = " << mean  << endl;
  cout << "VAR   = " << var   << endl;
  cout << "---------------------------" << endl;


// (3) Créer un masque
  image2d<bool> mask;
  initialize(mask, input);
  
  

// (4) Calculer les caractéristiques de chacunes des 2 classes pour chacun des seuils
  float   grp1_count[MAX_THRESHOLD];
  float   grp1_mean[MAX_THRESHOLD];  
  float   grp1_var[MAX_THRESHOLD];
  
  float   grp2_count[MAX_THRESHOLD];
  float   grp2_mean[MAX_THRESHOLD]; 
  float   grp2_var[MAX_THRESHOLD];

  typedef image_if< image2d<value::int_u8>, fun::eq_v2b_expr_< pw::value_< image2d<bool> >, pw::cst_<int> > > tmp_img_t;

  for (value::int_u8 threshold = min; threshold <= max; threshold++)
  {
    mask                  = binarization::threshold(input, threshold); 
    
    tmp_img_t        tmp1 = (input | (pw::value(mask) == false)).rw();
    grp1_count[threshold] = data::compute(accu::meta::math::count(), tmp1);    
    grp1_mean[threshold]  = data::compute(accu::meta::stat::mean(), tmp1);    
    grp1_var[threshold]   = data::compute(accu::stat::variance<float>(), tmp1);
#    grp1_var[threshold]   = data::compute(accu::stat::variance<value::int_u8>(), tmp2);
    
    tmp_img_t        tmp2 = (input | (pw::value(mask) == true)).rw();
    grp2_count[threshold] = data::compute(accu::meta::math::count(), tmp2);
    grp2_mean[threshold]  = data::compute(accu::meta::stat::mean(), tmp2);
    grp2_var[threshold]   = data::compute(accu::stat::variance<float>(), tmp2);
#    grp2_var[threshold]   = data::compute(accu::stat::variance<value::int_u8>(), tmp2);

    cout << "S = " << threshold << " ";
    cout << "G1=[" << grp1_count[threshold] << ", " << grp1_mean[threshold] << ", " << grp1_var[threshold] << "] ";
    cout << "G2=[" << grp2_count[threshold] << ", " << grp2_mean[threshold] << ", " << grp2_var[threshold] << "]" << endl;
  }
  
  cout << "---------------------------" << endl;


// (5) Recherche de la variance intra-classe minimale
// Problème d'analyse discriminante
// N      = N1+N2
// M      = (1/N)(N1*M1 + N2*M2)
// Vintra = (1/N)(N1*V1+N2*V2)
// Vinter = (1/N)(N1*(M-M1)² + N2*(M-M2)²)
// V      = Vinter + Vintra
  float         min_var       = var;
  value::int_u8 min_threshold = 0;

  for (value::int_u8 threshold = min; threshold <= max; threshold++)
  {
    float img_count = grp1_count[threshold] + grp2_count[threshold];
    float img_mean  = (grp1_count[threshold]*grp1_mean[threshold]+grp2_count[threshold]*grp2_mean[threshold])/img_count;
    float var_intra = (grp1_count[threshold]*grp1_var[threshold]+grp2_count[threshold]*grp2_var[threshold])/img_count;
    float grp1_diff = img_mean-grp1_mean[threshold];
    float grp2_diff = img_mean-grp2_mean[threshold];
    float var_inter = (grp1_count[threshold]*grp1_diff*grp1_diff+grp2_count[threshold]*grp2_diff*grp2_diff)/img_count;
    float img_var   = var_inter + var_intra;

    if (min_var > var_intra)
    {
      min_var       = var_intra;
      min_threshold = threshold;
    }

    cout << "S = " << threshold << " (" << img_count << ", " << img_mean << ", ";
    cout << img_var << "[" << var_intra << "|" << var_inter << "]) " << endl;
  }

  cout << " ==> MIN_VAR = " << min_var       << endl;
  cout << " ==> SEUIL   = " << min_threshold << endl;

// (6) Preparer l'image de sortie
  image2d<value::int_u8> output;
  initialize(output, input);
  data::paste(input, output);
  mask         = binarization::threshold(input, min_threshold);
  data::fill((output | (pw::value(mask) == false)).rw(), grp1_mean[min_threshold]);
  data::fill((output | (pw::value(mask) == true)).rw(),  grp2_mean[min_threshold]);

// binarisation
  mask = binarization::threshold(input, min_threshold);
  
// (7) Sauvegarder l'image résultante
  io::pgm::save(output, "./otsu.ppm");
  io::pbm::save(mask, "./otsu.pbm");

  trace::exiting("main");

  return 0;
}
