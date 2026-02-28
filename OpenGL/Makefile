my_opengl_app: src/main.cpp src/glad.c
	g++ src/main.cpp src/glad.c -I ./include -lglfw -lGL -ldl -o my_opengl_app -g -Wall -Werror

raytrace_test: src/raytrace/raytrace.cpp
	g++ src/raytrace/raytrace.cpp -o raytrace_test

clean:
	rm -f my_opengl_app raytrace_test *~ *.o core