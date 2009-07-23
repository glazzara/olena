// Pour les traces
#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

// Pour les entrées-sorties
#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

// Pour initialiser et remplir une image
#include <mln/make/image.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>

// Pour restreindre une image avec une fonction
#include <mln/core/image/dmorph/image_if.hh>

// Pour le reste
#include <mln/core/image/image2d.hh>


#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>
#include <mln/pw/value.hh>
#include <mln/fun/p2b/chess.hh>


// Le but est réaliser un opérateur construire l'image HelloWorld
// avec des couleurs différentes pour chacune des lettres
// L'image HelloWorld.pbm est supposé dans le répertoire courant


// Exemple I
// Charger image + restreindre coloriage ROI + sauvegarder image

void version1()
{
  using namespace mln;

  trace::entering("version1");
  trace::quiet = true;
  
  image2d<value::rgb8> rgb_img;
  io::ppm::load(rgb_img, "/usr/local/share/olena/images/small.ppm");
  
  box2d b(20,20);
  data::fill((rgb_img | b).rw(), literal::green);

  io::ppm::save(rgb_img, "./small_version1.ppm");

  trace::quiet = false;
  trace::exiting("version1");
}


// Exemple II
// Charger image + restreindre coloriage fonction + sauvegarder image

void version2()
{
  using namespace mln;

  trace::entering("version2");
  trace::quiet = true;
  
  image2d<value::rgb8> rgb_img;
  io::ppm::load(rgb_img, "/usr/local/share/olena/images/small.ppm");
  
  data::fill((rgb_img | fun::p2b::chess()).rw(), literal::green);

  io::ppm::save(rgb_img, "./small_version2.ppm");

  trace::quiet = false;
  trace::exiting("version2");
}

// Exemple III
// Charger image + restreindre coloriage masque + sauvegarder image

void version3()
{
  using namespace mln;

  trace::entering("version3");
  trace::quiet = true;
  
  image2d<value::rgb8> rgb_img;
  io::ppm::load(rgb_img, "/usr/local/share/olena/images/small.ppm");

  image2d<bool> bool_img;
  io::pbm::load(bool_img, "HelloWorld.pbm");
  
  image2d<bool> mask_img;
  initialize(mask_img, rgb_img);
  data::fill(mask_img, false);
  data::paste(bool_img, mask_img);
  
  data::fill((rgb_img | pw::value(mask_img)).rw(), literal::green);

  io::ppm::save(rgb_img, "./small_version3.ppm");

  trace::quiet = false;
  trace::exiting("version3");
}

// Exemple IV
// Charger image + restreindre coloriage predicat + sauvegarder image

void version4()
{
  using namespace mln;

  trace::entering("version4");
  trace::quiet = true;

  image2d<value::rgb8> rgb_img;
  io::ppm::load(rgb_img, "/usr/local/share/olena/images/small.ppm");

  image2d<bool> bool_img;
  io::pbm::load(bool_img, "HelloWorld.pbm");
  
  image2d<bool> mask_img;
  initialize(mask_img, rgb_img);
  data::fill(mask_img, false);
  data::paste(bool_img, mask_img);
  
  data::fill((rgb_img | pw::value(mask_img) == false).rw(), literal::green);

  io::ppm::save(rgb_img, "./small_version4.ppm");

  trace::quiet = false;
  trace::exiting("version4");
}

int main()
{
  using namespace mln;

  trace::quiet = false;

  trace::entering("main");

  version4();

  trace::exiting("main");

  return 0;
}
