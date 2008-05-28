#!/usr/bin/ruby

system("make icp")

plot_qe = File.new("log.dat", "w+")

for q in 1..10 do
  for e in 1..10 do

    t1 = Time.new
    system("./icp img/01.pbm img/02.pbm #{q} #{e}")
    t2 = Time.new
    
    print        "#{q} #{e} #{t2-t1}\n"
    plot_qe.puts "#{q} #{e} #{t2-t1}\n"

  end
end

plot_qe.close


plotscript = File.new("plotscript", "w+")
plotscript.puts "set xlabel \"q\""
plotscript.puts "set ylabel \"e\""
plotscript.puts "set zlabel \"time\""
plotscript.puts "splot \"log.dat\""
plotscript.puts "set terminal png nocrop enhanced size 800,600"
plotscript.puts "set output \"bench_qe.png\""
plotscript.puts "replot"
plotscript.puts "set output"
plotscript.puts "set terminal x11"
plotscript.close

exec("gnuplot plotscript")




