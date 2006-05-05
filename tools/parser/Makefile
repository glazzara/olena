SDF = $(wildcard grammar/*.sdf)
OUT = Scool

all: Scool.def Scool.tbl Scool.str

Scool.def: $(SDF)
	pack-sdf -i grammar/Scool.sdf -o $@

Scool.tbl: Scool.def
	sdf2table -i $< -o $@ -m Scool

Scool.rtg: Scool.def
	sdf2rtg -i $< -m Scool -o $@

Scool.str: Scool.rtg
	rtg2sig --module Scool -i $< -o $@

clean:
	rm -rf Scool.* log.*

