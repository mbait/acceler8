SRC_DIR = data
FONT = terminus, 9
GSIZE = 800,300
LINE_STYLE = pt 5 ps 0.5 lt 1 lw 0.85

all: $(patsubst %/, %.png, $(wildcard ${SRC_DIR}*/))

data%.png : ${SRC_DIR}%
	perl gen_graph.pl $< | gnuplot > $@

clean :
	$(RM) *.png

.SUFFIXES :
