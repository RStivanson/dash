OBJS = dash

all: ${OBJS}

dash:	flex.l bison.y Func.o ProcList.o Execute.o Directory.o Display.o
	flex flex.l
	bison -d bison.y --debug
	cc -o $@ -g bison.tab.c lex.yy.c Func.o ProcList.o Execute.o Directory.o Display.o

Func.o: Func.h
ProcList.o: ProcList.h
Execute.o: Execute.h ProcList.o Func.o Directory.o
Directory.o: Directory.h Func.o
Display.o: Display.h Directory.o

clean:
	rm ${OBJS} bison.tab.* lex.yy.* *.o *~