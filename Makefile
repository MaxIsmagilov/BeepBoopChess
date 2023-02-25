all: 
	gcc -oFast Main.cpp Uciapi.hh NegaMax.hh Eval.hh Moves.hh Board.hh Tools.hh MagicNumbers.hh -o Main -lstdc++

debug: 
	gcc -o Main.cpp Uciapi.hh NegaMax.hh Eval.hh Moves.hh Board.hh Tools.hh MagicNumbers.hh -o Main

clean:
	rm -f *.o
	rm -f *.exe
	
	