COMP = gcc
EDL = gcc
ECHO = echo
RM =rm
CPPFLAG = 

PATH_LIB= 
PATH_H= 
SRC= $(wildcard *.c)
H= $(wildcard *.h)
OBJETS = $(SRC:.c=.o)

EXE = Philosophe
EFFACE = clean
LIBS= -L$(PATH_LIB)
${EXE}:${OBJETS}
	${ECHO} "edl de ${EXE}"
	${EDL} -o ${EXE} ${OBJETS} #${LIBS}

%.o:%.c
	${ECHO} "compile de <$<>"
	${COMP} -c $<
Main.o:Main.c

${EFFACE}:
	${RM} ${EXE} ${OBJETS}
