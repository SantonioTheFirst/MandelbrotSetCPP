output: Mandelbrot.cpp
	g++ Mandelbrot.cpp -fopenmp -O3 -fno-signed-zeros -fno-trapping-math -funroll-loops -frename-registers -fprofile-generate -march=native -o Mandelbrot
	./run.sh
	g++ Mandelbrot.cpp -fopenmp -O3 -fno-signed-zeros -fno-trapping-math -funroll-loops -frename-registers -fprofile-use -fprofile-correction -march=native -o Mandelbrot
	rm *.gcda

clear:
	rm *.gcda