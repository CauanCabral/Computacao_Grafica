build:
	gcc-4.1 -D__LINUX -s -O2 -o Main *.cpp -lGL -lGLU -lglut

clean:
	rm -f *.out Main
