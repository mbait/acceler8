SRC_DIR = data
FONT = terminus, 9
GSIZE = 800,300
LINE_STYLE = pt 5 ps 0.5 lt 1 lw 0.85

PLOTS = $(patsubst %/, %.png, $(wildcard ${SRC_DIR}*/))
HISTS = $(patsubst %.pl, %.png, $(wildcard hist*.pl))

all: ${PLOTS} ${HISTS}

data%.png : ${SRC_DIR}%
	perl gen_graph.pl $< | gnuplot > $@

hist%.png : hist%.pl
	perl $< | gnuplot > $@

clean :
	$(RM) *.png

.SUFFIXES :
