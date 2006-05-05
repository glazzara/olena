all:
	cd parser && make
	cd transformer && make

clean:
	rm -rf log.*
	cd parser && make clean
	cd transformer && make clean

check: all
	uttk test.yml

gen: all
	uttk gen.yml
