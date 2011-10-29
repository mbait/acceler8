NAME=msp
PID_FILE=batch.pid
BUILD_DIR=out
BUILD_PATH=$(BUILD_DIR)/$(NAME)
DISR_NAME=Acceler8-Alexander_Solovets

SOURCE=src/$(NAME).c

# 'debug' is default target
debug :
	CFLAGS="-ggdb -mssse3 -DDEBUG" $(MAKE) $(BUILD_PATH)_dbg

optimized :
	CFLAGS="-O2" $(MAKE) $(BUILD_PATH)_opt

simd :
	CFLAGS="-mssse3" $(MAKE) $(BUILD_PATH)_sse

openmp :
	CFLAGS="-fopenmp" $(MAKE) $(BUILD_PATH)_omp

retail :
	CFLAGS="-fopenmp -O2" $(MAKE) $(BUILD_PATH)_rtl

extreme :
	CFLAGS="-fopenmp -O3 -mssse3" $(MAKE) $(BUILD_PATH)_xtm

vector:
	CFLAGS="-fopenmp -O2 -mssse3" $(MAKE) $(BUILD_PATH)_vec

default :
	$(MAKE) $(BUILD_PATH)_nop

# Compile sources into object file and save asm listing.
$(BUILD_PATH)%.o : $(SOURCE)
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -g -c -Wa,-a,-ad $< > $(@:.o=.lst) -o $@
# Link object file and create executable.
$(BUILD_PATH)% : $(BUILD_PATH)%.o
	$(CC) $(CFLAGS) $< -o $@

all : debug optimized openmp retail extreme simd default vector

clean:
	$(RM) $(BUILD_DIR)/*

dist: retail doc
	git archive --prefix=$(DISR_NAME)/ HEAD | gzip > $(DISR_NAME).tgz

batch:
	CC=/opt/gcc/4.5.1/bin/gcc \
	   LD_LIBRARY_PATH=/opt/mpc/lib:/opt/mpfr/lib/:/opt/gmp/lib \
	   $(MAKE) clean all
	$(RM) -r ${PID_FILE}
	./sched 1 >> ${PID_FILE}
	./sched 4 >> ${PID_FILE}
	./sched 8 >> ${PID_FILE}
	./sched 16 >> ${PID_FILE}
	./sched 35 >> ${PID_FILE}
	./sched 40 >> ${PID_FILE}
	cat ${PID_FILE}

doc:


.SUFFIXES :

.PHONY : clean debug optimized openmp retail extreme simd
