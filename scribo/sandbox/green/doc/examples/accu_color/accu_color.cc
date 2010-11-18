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
#include <mln/accu/stat/var.hh>
#include <mln/data/compute.hh>

// Pour les chargements/sauvegardes des images
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

// Pour les types de base
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/lut_vec.hh>
#include <mln/value/set.hh>
#include <mln/literal/colors.hh>

// Pour le seuillage d'une image
#include <mln/binarization/threshold.hh>

// Pour le mécanisme de restriction de domaine
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/concept/meta_accumulator.hh>
#include <mln/pw/value.hh>

// Pour le copier/coller et le remplissage
#include <mln/data/paste.hh>
#include <mln/data/fill.hh>
#include <mln/make/pix.hh>
#include <mln/make/vec.hh>

// Pour les macros basiques (affectation d'une image ...)
#include <mln/core/var.hh>
#include <mln/algebra/vec.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/concept/value_set.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/trait/images.hh>


# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>
# include <mln/value/lut_vec.hh>
# include <mln/opt/value.hh>
//
// DEFINITION D'UNE FONCTION COMPUTE POUR L'ACCUMULATOR MATRICE VARIANCE/COVARIANCE
//
namespace mln
{
  namespace usr
  {
    template<typename A, typename I, typename F>
    A
    compute(Accumulator<A>& a_, const Image<I>& input_, const Function_v2v<F>& fn_)
    {
      A&    a        = exact(a_);
      const I& input = exact(input_);
      const F& fn    = exact(fn_); 

      // vérifier la concordance entre l'image et l'accumulateur
      mln_piter(I) p(input.domain());
      for_all(p)
        a.take(fn(input(p)));

      return a;
    }
  } // namespace usr
} // namespace mln


//
// DEFINTIION D'UNE FONCTION TRADUISANT UN PIXEL RGB EN VECTEUR 3D
//
namespace mln
{
  namespace fun
  {
    namespace usr
    {
      template<typename V, typename R>
      struct rgb2vec : public Function_v2v< rgb2vec<V,R> >
      {
        typedef R result;
        result operator()(const V& rgb) const;
      };

      template<typename V, typename R>
      typename rgb2vec<V,R>::result
      rgb2vec<V,R>::operator()(const V& rgb) const
      {
        // !!la dimension des objets doit être la même!!

        return make::vec(rgb.red(), rgb.green(), rgb.blue());
      }
    } // namespace usr
  } // namespace fun
} // namespace mln


//
// DEFINTIION D'UNE FONCTION IMAGE EXISTANE ASSOCIANT UNE VALEUR A UN SITE
//
namespace mln
{
  namespace fun
  {
    namespace usr
    {
      template<typename I>
      struct image_fun : public Function_v2v< image_fun<I> >, private mlc_is_a(I, Image)::check_t
      {
        typedef mln_value(I) result;
        image_fun(mlc_const(I)& img);
        result operator()(const mln_psite(I)& p) const;

        private:
        mlc_const(I) img_;
      };

      template<typename I>
      image_fun<I>::image_fun(mlc_const(I)& img) : img_(img)
      {
      }

      template<typename I>
      typename image_fun<I>::result
      image_fun<I>::operator()(const mln_psite(I)& p) const
      {
        return img_(p);
      }
    } // namespace usr
  } // namespace fun
} // namespace mln


namespace mln{namespace fun{

template<typename I, typename F>
void test(Image<I>& input_, const Function_v2v<F>& f_)
{
  I&    input = exact(input_);
  const F& f  = exact(f_);

  value::lut_vec<mln_vset(I), mln_result(F)> lut(input.values_eligible(), f);

  std::cout << lut;
}
}}

template<typename V>
void print(V v)
{
  using namespace mln;
  using namespace std;
  
  mln_trait_image_quant(image2d<value::rgb8>) quant;
  cout << quant.name();
}

int main()
{
  using namespace mln;
  using namespace std;

  typedef algebra::vec<3,float> v3i;

  image2d<value::rgb8> input;
  io::ppm::load(input, "/usr/local/share/olena/images/small.ppm");

  accu::stat::var<v3i>                  var;
  value::rgb8                           rgb(8,2,4);
  fun::usr::rgb2vec< value::rgb8, v3i > fun;
  v3i                                   aVector;

  //usr::compute2(accu::stat::var<v3i>(), input, fun::usr::rgb2vec< value::rgb8, v3i >());
  //data::compute(accu::stat::var<v3i>(), input);
  //cout << usr::compute(var, input, fun);

   mln::fun::test(input, fun:v2v::inc());
}

/*
int main2()
{
  using namespace mln;
  using namespace std;

//  trace::quiet = false;
  trace::entering("main");

  // (1) Charger une image
  image2d<value::rgb8> input;
  io::ppm::load(input, "/usr/local/share/olena/images/small.ppm");

  typedef algebra::vec<3,value::int_u8> v3i;

  accu::stat::var<v3i>                  var;
  value::rgb8                           rgb(8,2,4);
  fun::usr::rgb2vec< value::rgb8, v3i > fun;
  v3i                                   aVector;

  aVector[0] = 8;
  aVector[1] = 2;
  aVector[2] = 4;

  //var.take(aVector);
  //var.take(make::vec(rgb.red(), rgb.green(), rgb.blue()));

  //value::rgb8 purple  = literal::purple;
  value::rgb8 purple  = aVector;

  cout << "( " << purple.red() << " , " << purple.green() << " , " << purple.blue() << " )" << endl;
  
  
  //usr::compute2(accu::stat::var<v3i>(), input, fun::usr::rgb2vec< value::rgb8, v3i >());
  cout << usr::compute(var, input, fun);
  // tester s'il peut trouver la fonction


  // Essaie avec  using namespace mln;
  using namespace std;
  //look up table pour les couleurs
  fun::usr::image_fun< image2d<value::rgb8> >  image_fun(input);

  print<int>(3);
  mln::fun::test(input, fun:v2v::inc());

  //value::lut_vec<mln_vset(image2d<value::rgb8>), mln_result(fun::usr::image_fun< image2d<value::rgb8> >) >           lut(input.values_eligible(), image_fun);

  trace::exiting("main");

  return 0;
}
*/

