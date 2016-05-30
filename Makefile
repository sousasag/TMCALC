# makefile 
# variaveis do make
INCLUDES =  
SOURCES = tmcalc.c TMCalc.bash
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

tmcalc.o: tmcalc.c lineRatioCalibCore.h

TMCalc.bash: feh_calib_lines.dat ratios_list.dat

clean: 
	rm ${OBJFILES} tmcalc


