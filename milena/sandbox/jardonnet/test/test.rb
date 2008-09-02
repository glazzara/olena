#!/usr/bin/ruby

print "#  icp_ref \t icp\n"
print "-------------------------\n"

q = 4
e = 5

for i in 0..9 do

  # icp_ref

  t1 = Time.new
  system("./icp_ref img/c#{i}.pbm img/x#{i}.pbm #{q} #{e}")
  t2 = Time.new
  t = t2 - t1

  ta1 = Time.new
  system("./icp_ref img/c#{i}.pbm img/x#{i}.pbm #{q} #{e}")
  ta2 = Time.new
  ta = ta2 - ta1

  tb1 = Time.new
  system("./icp_ref img/c#{i}.pbm img/x#{i}.pbm #{q} #{e}")
  tb2 = Time.new
  tb = tb2 - tb1

  # icp

  tt1 = Time.new
  system("./icp img/c#{i}.pbm img/x#{i}.pbm #{q} #{e}")
  tt2 = Time.new
  tt = tt2 - tt1

  tta1 = Time.new
  system("./icp img/c#{i}.pbm img/x#{i}.pbm #{q} #{e}")
  tta2 = Time.new
  tta = tta2 - tta1

  ttb1 = Time.new
  system("./icp img/c#{i}.pbm img/x#{i}.pbm #{q} #{e}")
  ttb2 = Time.new
  ttb = ttb2 - ttb1

  # mean

  meant  = (t  + ta  + tb) / 3
  meantt = (tt + tta + ttb) /3

  print "#{i}  #{t} \t #{tt}\n"
end
