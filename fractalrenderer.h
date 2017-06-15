#ifndef FRACTALRENDERER_H
#define FRACTALRENDERER_H

#include <QImage>
#include <iostream>
#include <inttypes.h>
#include "complex.h" //erst mal
//#include <complex>

typedef unsigned uint;
using std::cout;
using std::endl;

struct Color
{
    Color(uint8_t nr,uint8_t ng,uint8_t nb) : r(nr), g(ng), b(nb){}
    Color(){}
    uint8_t r=0,g=0,b=0;
};

class FractalRenderer
{
    public:
        FractalRenderer(uint rsizex, uint rsizey);
        ~FractalRenderer();
        void render();
        void render_mandelbrot(double top,double left,double down,double right);
        void render_julia(Complex c,double top,double left,double down,double right);
        void move_window(int xmove,int ymove,double top,double left,double down,double right);
        QImage * export_picture();
        void resize(uint newx,uint newy);
        void set_mod(int newmod);

    private:
        uint m_renderx,m_rendery;
        uint m_max_x,m_max_y;
        int modulator = 20;
        Color ** m_result;
        Complex m_lastpoint;
        int calculate_point(Complex start, Complex point) const;
        Complex mandelbrot_recursive(Complex z,Complex c) const;
};

#endif // FRACTALRENDERER_H
