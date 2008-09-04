# include <mln/core/image/image2d.hh>
# include <mln/core/window2d.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/win/fun.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>


// # include <mln/morpho/gradient.bis.hh>
// # include <mln/morpho/erosion.bis.hh>

// # include <mln/value/int_u8.hh>
// # include <mln/io/pgm/load.hh>
// # include <mln/io/pgm/save.hh>


template <typename I, typename W, typename P>
void picture(const I& ima, const W& win, const P& p)
{
  std::cout << ima(p) << ": ";
  mln_qiter(W) q(win, p);
  for_all(q)
    if (ima.has(q))
      std::cout << ima(q) << ' ';
    else 
      std::cout << "- ";
  std::cout << std::endl;
}



struct uni_t
{
  typedef mln::window2d result;
  mln::window2d win_;
  uni_t() { win_.insert(-1,0).insert(0,-1); }
  const mln::window2d& operator()(const mln::point2d& p) const { return win_; }
} uni;


struct bi_t
{
  typedef mln::window2d result;
  mln::window2d win1_, win2_;
  bi_t() {
    win1_.insert(-1,0).insert(0,-1);
    win2_.insert( 1,0).insert(0, 1);
  }
  const mln::window2d& operator()(const mln::point2d& p) const {
    return p.col() % 2 ? win1_ : win2_;
  }
} bi;


struct bi_rect_t
{
  typedef mln::win::rectangle2d result;
  result *win1_, *win2_;
  bi_rect_t() {
    win1_ = new result(1,3);
    win2_ = new result(3,1);
  }
  const result& operator()(const mln::point2d& p) const {
    return p.row() % 2 ? *win1_ : *win2_;
  }
} bi_rect;


// template <typename T>
// struct grad_t
// {
//   typedef mln::win::rectangle2d result;
//   mln::image2d<T> grad_;
//   std::vector<result*> win_;

//   grad_t(const mln::image2d<T>& gr)
//   {
//     grad_ = gr;
//     for (unsigned i = 0; i < 10; ++i)
//       {
// 	unsigned l = i % 2 ? i : i + 1;
// 	win_.push_back(new result(l, l));
//       }
//   }
//   const result& operator()(const mln::point2d& p) const {
//     float s = 1.f - grad_(p) / 256.f;
//     s *= s * s;
//     return * win_[unsigned(s * 9)];
//   }
// };




int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(2, 3, 0); // no border
  debug::iota(ima);
  debug::println(ima);


  {
    win::fun<uni_t> win(uni);
    mln_fwd_piter_(I) p(ima.domain());
    for_all(p)
      picture(ima, win, p);
    std::cout << std::endl;
  }

  {
    win::fun<bi_t> win(bi);
    mln_fwd_piter_(I) p(ima.domain());
    for_all(p)
      picture(ima, win, p);
    std::cout << std::endl;
  }

  {
    win::rectangle2d rect(3, 3);
    mln_fwd_piter_(I) p(ima.domain());
    for_all(p)
      picture(ima, rect, p);
    std::cout << std::endl;
  }

  {
    win::fun<bi_rect_t> win(bi_rect);
    mln_fwd_piter_(I) p(ima.domain());
    for_all(p)
       picture(ima, win, p);
    std::cout << std::endl;
  }



//   {
//     using value::int_u8;
//     image2d<int_u8> ima(2, 2);
//     // io::pgm::load(ima, "small.pgm");

//     window2d c4p;
//     c4p.insert(-1,0).insert(0,-1).insert(0,0).insert(0,1).insert(1,0);

//     grad_t<int_u8> f(morpho::gradient(ima, c4p));
//     win::fun< grad_t<int_u8> > win(f);

//     image2d<int_u8> ero = morpho::erosion(ima, win);
//     // io::pgm::save(ero, "ero.pgm");

//     mln_fwd_piter_(I) p(ima.domain());
//     for_all(p)
//       picture(ima, win, p);
//     std::cout << std::endl;
//   }

}
