#!/bin/bash
./Mandelbrot
pnmtopng output.ppm > output.png
rm *.ppm