OBJS = main.cpp asembler.cpp Symbol.cpp sekcija.cpp redSekcije.cpp redRelTabele.cpp

prog: $(OBJS)
	g++ -std=c++11 -gdwarf-2 $(OBJS) -o proga


clean: 
	rm *^(\.cpp$|\.h$) asm
