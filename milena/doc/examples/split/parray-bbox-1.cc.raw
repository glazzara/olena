  p_array<point2d> arr;
  arr.insert(point2d(1,0));
  arr.insert(point2d(1,1));

  // The bbox is computed thanks to bbox() algorithm.
  box2d box = geom::bbox(arr);
  std::cout << box << std::endl;

  // p_array provides nsites(),
  // it can be retrieved in constant time.
  std::cout << "nsites = " << arr.nsites() << std::endl;
