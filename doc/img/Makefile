FONT = terminus, 9
GSIZE = 800,300
LINE_STYLE = pt 5 ps 0.5 lt 1 lw 0.85

TARGETS = turn_matrix remove_critical

all: ${TARGETS:=.png}

%.png : %
	perl gen_graph.pl $< | gnuplot > $@

clean :
	$(RM) *.png

.SUFFIXES :
