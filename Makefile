all: 
	gcc -oFast src/Main.cpp src/Uciapi.hh src/NegaMax.hh src/Eval.hh src/Moves.hh src/Board.hh src/Tools.hh src/MagicNumbers.hh -o bin/Main -lstdc++ -lthread

debug: 
	gcc -o src/Main.cpp src/Uciapi.hh src/NegaMax.hh src/Eval.hh src/Moves.hh src/Board.hh src/Tools.hh src/MagicNumbers.hh -o bin/Main -lstdc++ -lthread

run:
	./bin/Main

clean:
	rm -f bin/*.o
	rm -f bin/*.exe
	rm -f src/*.exe
	rm -f *.exe

	
	