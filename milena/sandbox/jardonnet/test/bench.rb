#!/usr/bin/ruby

system("make icp")

plot_qe = File.new("log.dat", "w+")

for q in 1..10 do
  for e in 1..10 do

    t1 = Time.new
    system("./icp 01.pbm 02.pbm #{q} #{e}")
    t2 = Time.new
    
    print        "#{q} #{e} #{t2-t1}\n"
    plot_qe.puts "#{q} #{e} #{t2-t1}\n"

  end
end

plot_qe.close


plotscipt = File.new("plotscript", "w+")
plotscipt.puts "set xlabel \"q\""
plotscipt.puts "set ylabel \"e\""
plotscipt.puts "set zlabel \"time\""
plotscipt.puts "splot \"log.dat\""
plotscipt.puts "pause -1"
plotscipt.close

exec("gnuplot plotscript")


