SEED ?= 0
BYTE ?= 0
LOGN ?= 29


all: runall

ex1_basic: ex1a.c helpers.h
	$(CC) -g -Wall -Wextra -O0 -std=c11 -DSEED=$(SEED) -o $@ ex1a.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex1_O3: ex1a.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -mno-sse -std=c11 -DSEED=$(SEED) -o $@ ex1a.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex1_march_native: ex1a.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -march=native -std=c11 -DSEED=$(SEED) -o $@ ex1a.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex1_specify_alignment: ex1b.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -march=native -std=c11 -DSEED=$(SEED) -o $@ ex1b.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex1_manual: ex1c.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -march=native -std=c11 -DSEED=$(SEED) -o $@ ex1c.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex1_manual_unroll: ex1d.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -march=native -std=c11 -DSEED=$(SEED) -o $@ ex1d.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump


ex2_basic: ex2a.c helpers.h
	$(CC) -g -Wall -Wextra -O0 -std=c11 -DSEED=$(SEED) -o $@ ex2a.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex2_O3: ex2a.c helpers.h
	$(CC) -g -Wall -Wextra -O3  -mno-sse -std=c11 -DSEED=$(SEED) -o $@ ex2a.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex2_march_native: ex2a.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -march=native -std=c11 -DSEED=$(SEED) -o $@ ex2a.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump

ex2_manual: ex2b.c helpers.h
	$(CC) -g -Wall -Wextra -O3 -march=native -std=c11 -DSEED=$(SEED) -o $@ ex2b.c
	gdb -batch -ex "disassemble/rs count_pairs" $@ > $@.dump


clean:
	rm -f ex1_basic ex1_O3 ex1_march_native ex1_specify_alignment ex1_manual ex1_manual_unroll ex2_basic ex2_O3 ex2_march_native ex2_manual perf.data perf.data.old

runall: ex1_basic ex1_O3 ex1_march_native ex1_specify_alignment ex1_manual ex1_manual_unroll  ex2_basic ex2_O3 ex2_march_native ex2_manual 
	@echo "### Solving with Aligned Pairs";
	ex1_basic $(BYTE) $(LOGN)
	@echo "";
	ex1_O3 $(BYTE) $(LOGN)
	@echo "";
	ex1_march_native $(BYTE) $(LOGN)
	@echo "";
	ex1_specify_alignment $(BYTE) $(LOGN)
	@echo "";
	ex1_manual $(BYTE) $(LOGN)
	@echo "";
	ex1_manual_unroll $(BYTE) $(LOGN)
	@echo "";
	@echo "### Solving with Unaligned Pairs";
	ex2_basic $(BYTE) $(LOGN)
	@echo "";
	ex2_O3 $(BYTE) $(LOGN)
	@echo "";
	ex2_march_native $(BYTE) $(LOGN)
	@echo "";
	ex2_manual $(BYTE) $(LOGN)