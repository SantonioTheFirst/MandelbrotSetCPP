#include <iostream>
#include <fstream>
#include <omp.h>
#include <chrono>

using namespace std;


uint8_t MandelbrotPoint(uint16_t x, uint16_t y, float border_sq);
uint8_t** Mandelbrot(uint16_t maxiter, float border, uint16_t w, uint16_t h, float pmin, float pmax, float qmin, float qmax);
int main(int argc, char** argv);


uint8_t** Mandelbrot(uint16_t maxiter, float border, uint16_t w, uint16_t h, float pmin, float pmax, float qmin, float qmax){
    float* ppoints = new float[w];
    float* qpoints = new float[h];
    uint8_t **image = new uint8_t *[h];
    float border_sq = border * border;

    float pstep = (pmax - pmin) / w;
    float qstep = (qmax - qmin) / h;

    ppoints[0] = pmin;
    qpoints[0] = qmin;

    for (uint16_t i = 0; i < h; i++)
    {
        if (i != 0)
        {
            ppoints[i] = ppoints[i - 1] + pstep;
            qpoints[i] = qpoints[i - 1] + qstep;
        }
        image[i] = new uint8_t[w];
    }

    #pragma omp parallel for
    for (uint16_t i = 0; i < h; i++)
    {
        for (uint16_t j = 0; j < w; j++)
        {
            float realC = ppoints[j];
            float imC = qpoints[i];
            float realZ = 0;
            float imZ = 0;
            float rZ = realZ;
            float iZ = imZ;
            uint16_t iter = 0;
            while (((realZ * realZ + imZ * imZ) < border_sq) && (iter < maxiter))
            {
                rZ = realZ;
                iZ = imZ;
                realZ = rZ * rZ - iZ * iZ + realC;
                imZ = 2 * rZ * iZ + imC;
                iter++;
            }
            uint8_t value = 0;
            if (iter == maxiter)
                value = 0;
            else
                value = (iter * 255) / maxiter;
            image[i][j] = value;
        }
    }

    return image;
}



int main(int argc, char** argv){
    const uint16_t w = 5000;
    const uint16_t h = w;
    const uint16_t maxiter = 1000;
    float pmin = -2.5;
    float pmax = 1.5;
    float qmin = -2.0;
    float qmax = 2.0;
    float border = 2.0;
    float x = -0.793197078177363;
    float y = 0.16093721735804;
    pmin = x - 0.01;
    pmax = x + 0.01;
    qmin = y - 0.01;
    qmax = y + 0.01;
    const float border_sq = border * border;
    uint8_t** image;

    auto start = chrono::high_resolution_clock().now();
    image = Mandelbrot(maxiter, border, w, h, pmin, pmax, qmin, qmax);

    ofstream output("output.ppm");

    if (output.is_open())
    {
        output << "P3\n"
               << (uint16_t)w << " " << (uint16_t)h << " 1024\n";
        for (uint16_t i = 0; i < h; i++)
        {
            for (uint16_t j = 0; j < w; j++)
            {
                output << ((uint16_t)image[i][j] + 1) * 4 << ' ' << 0 << ' ' << 0 << "\n";
            }
        }
        output.close();
    }
    else
        cout << "Could not open the file";

    auto stop = chrono::high_resolution_clock().now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << endl;
    
    for (uint16_t i = 0; i < h; i++)
    {
        delete [] image[i];
    }
    delete [] image;

    return 0;
}