all: growcut remove_if_blue growcut_bw keep_color

growcut: main.cc
	g++ -O2 -DNDEBUG -I ../../.. main.cc -o growcut

growcut_bw: main_bw.cc
	g++ -O2 -DNDEBUG -I ../../.. main_bw.cc -o growcut_bw

remove_if_blue:
	g++ -O2 -DNDEBUG -I ../../.. remove_if_blue.cc -o remove_if_blue

keep_color: keep_color.cc
	g++ -O2 -DNDEBUG -DCOLOR=255 -I ../../.. keep_color.cc -o keep_white
	g++ -O2 -DNDEBUG -DCOLOR=70 -I ../../.. keep_color.cc -o keep_grey
	g++ -O2 -DNDEBUG -DCOLOR=1 -I ../../.. keep_color.cc -o keep_black

debug:
	g++ -g3 -ggdb3 -I ../../.. main.cc -o growcut

check: growcut_bw keep_color
	./check.sh

clean:
	rm -rf obj
	rm -rf remove_if_blue
	rm -rf growcut
	rm -rf growcut_bw
	rm -rf keep_white
	rm -rf keep_grey
	rm -rf keep_black
