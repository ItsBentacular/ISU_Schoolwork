assignment01: curse_main.cpp heap.c heap.h poke_logic.hpp poke_logic.cpp poke_data.hpp poke_data.cpp poke_interior.cpp poke_interior.hpp
	g++ curse_main.cpp heap.c poke_logic.cpp poke_data.cpp poke_interior.cpp -o assignment01 -g -Wall -Werror -lncurses

clean:
	rm -f assignment01 *~ *.o core

debug: curse_main.cpp heap.c heap.h poke_logic.hpp poke_logic.cpp poke_data.hpp poke_data.cpp
	g++ -fsanitize=address -fno-omit-frame-pointer \
	curse_main.cpp heap.c poke_logic.cpp poke_data.cpp \
	-o assignment01 -g -Wall -Werror -lncurses
