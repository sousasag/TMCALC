# makefile 
# variaveis do make
INCLUDES =  
SOURCES = tmcalc.c TMCalc_repo.bash
OBJFILES = tmcalc.o 

# regras de sufixo
.SUFFIXES : .c .o

# como transformar um .c num .o ; $< ­­-- nome do ficheiro
.c.o:
	gcc -g -ansi -Wall -c -lm $<  

# o eclipse assume que o target por defeito chama­se "all"
all: tmcalc

# para fazer o programa principal 
tmcalc: ${OBJFILES}
	gcc -g -ansi -Wall -o tmcalc ${OBJFILES} -lm
	sed "4s@.*@cd $(CURDIR)@" TMCalc_repo.bash > TMCalc.bash
	chmod a+x TMCalc.bash
	cd tmcalc_cython; python setup.py build_ext --inplace

tmcalc.o: tmcalc.c lineRatioCalibCore.h

TMCalc.bash: feh_calib_lines.dat ratios_list.dat

clean: 
	rm ${OBJFILES} tmcalc
<<<<<<< HEAD
	cd tmcalc_cython; rm -rf build tmcalc_modul*.so tmcalc_module.c
=======
	rm -rf TMCalc.bash
	cd tmcalc_cython; rm -rf build tmcalc_module.so tmcalc_module.c
>>>>>>> 7ad69a91f8487ded67828f1857467337b0f1ab73


