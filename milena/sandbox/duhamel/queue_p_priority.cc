#include "queue_p_priority.hh"
#include <mln/core/image/image2d.hh>

int main ()
{
  using namespace mln;

  mln::queue_p_priority<point2d, unsigned> q;
  point2d p1 (1,1);
  point2d p2 (1,5);
  point2d p3 (2,7);

  mln_assertion (q.empty ());

  mln_assertion (q.npoints () == 0);

  q.push_force (p3);
  q.push_force (p1, 3);
  q.push_force (p2, 5);


  mln_assertion (!q.empty ());

  mln_assertion (q.has (p1));
  mln_assertion (q.has (p2));
  mln_assertion (q.has (p3));

  mln_assertion (q.npoints () == 3);
  mln_assertion (q.front () == p2);
  q.pop ();

  mln_assertion (q.has (p1));
  mln_assertion (!q.has (p2));
  mln_assertion (q.has (p3));

  mln_assertion (q.npoints () == 2);
  mln_assertion (q.front () == p1);
  q.pop ();

  mln_assertion (!q.has (p1));
  mln_assertion (!q.has (p2));
  mln_assertion (q.has (p3));

  mln_assertion (q.npoints () == 1);
  mln_assertion (q.front () == p3);
  q.pop ();

  mln_assertion (!q.has (p1));
  mln_assertion (!q.has (p2));
  mln_assertion (!q.has (p3));
  mln_assertion (q.npoints () == 0);

  mln_assertion (q.empty ());

  q.push_force (p3);
  q.push_force (p2, 5);
  q.push_force (p1, 3);

  mln_assertion (q[0] == p3);
  mln_assertion (q[1] == p1);
  mln_assertion (q[2] == p2);
  q.clear ();
  mln_assertion (q.empty ());
}
