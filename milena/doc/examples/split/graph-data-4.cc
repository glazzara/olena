  typedef fun::i2v::array<point2d> F;
  F f(5); // We need to map 5 vertices.
  f(0) = point2d(0, 0);
  f(1) = point2d(2, 2);
  f(2) = point2d(0, 4);
  f(3) = point2d(4, 3);
  f(4) = point2d(4, 4);
