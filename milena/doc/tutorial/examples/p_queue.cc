# include <mln/core/image2d.hh>
# include <mln/core/p_queue_fast.hh>
# include <mln/debug/println.hh>
# include <mln/level/fill.hh>

# include <mln/core/p_priority_queue.hh>



template <typename S>
void picture(const S& s)
{
  using namespace mln;

  image2d<char> ima(5, 5);
  level::fill(ima, '-');
  
  unsigned i = 0;
  mln_piter(S) p(s);
  for_all(p)
    ima(p) = '0' + i++;

  debug::println(ima);
}



int main()
{
  using namespace mln;
  
  typedef p_queue_fast<point2d> Q;
  Q q;

  {
    point2d p(1,1);
    q.push(p);
    dpoint2d dp[] = { right, right, down, down, left, left, up };
    for (unsigned i = 0; i < 7;  ++i)
      p += dp[i], q.push(p);
  }

  picture(q);
  q.pop();
  picture(q);

  {
    Q::psite p(q, 0);
    mln_assertion(q.has(p));
    p.change_index(6);
    mln_assertion(q.has(p));

    p.change_index(7);
    mln_assertion(! q.has(p));
  }

  std::cout << "purge!" << std::endl;
  q.purge();
  picture(q);

  {
    typedef p_priority<unsigned, Q> PQ;
    PQ pq;
    Q::piter p(q);
    for_all(p)
      pq.push(p.row() + p.col(), p);
    std::cout << pq << std::endl;
  }

}
