all: 
	gcc -oFast src/Main.cpp src/Uciapi.hh src/NegaMax.hh src/Eval.hh src/Board.hh src/MagicNumbers.hh -o bin/Main -lstdc++ 

debug: 
	gcc -o src/Main.cpp src/Uciapi.hh src/NegaMax.hh src/Eval.hh ssrc/Board.hh src/MagicNumbers.hh -o bin/Main -lstdc++ 

run:
	./bin/Main

clean:
	rm -f bin/*.o
	rm -f bin/*.exe
	rm -f src/*.exe
	rm -f *.exe

	
	