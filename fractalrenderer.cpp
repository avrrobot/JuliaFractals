#include "fractalrenderer.h"

FractalRenderer::FractalRenderer(uint rsizex, uint rsizey)
                    : m_renderx(rsizex), m_rendery(rsizey), m_max_x(rsizex), m_max_y(rsizey)
{
    m_result = new Color*[rsizex];
    for(uint i = 0;i<rsizex;i++)
    {
        m_result[i] = new Color[rsizey];
    }
}

FractalRenderer::~FractalRenderer()
{
    for(uint i = 0;i<m_renderx;i++)
    {
        delete [] m_result[i];
    }
    delete [] m_result;
}
#define sign(x) ((x > 0) - (x < 0))
void FractalRenderer::move_window(int xmove,int ymove,double top,double left,double down,double right)
{
    bool is_julia = (m_lastpoint.r != -100);
    if(xmove == 0 && ymove == 0)
        return;
    int posx = 0,posy = 0;
    if(xmove > 0)
    {
        posx = m_renderx-1;
    }
    if(ymove > 0)
    {
        posy = m_rendery-1;
    }
    posx -= xmove;
    posy -= ymove;
    int starty = posy;
    int xstep = -sign(xmove);
    if(xstep == 0)
        xstep = 1;
    int ystep = -sign(ymove);
    if(ystep == 0)
        ystep = 1;
    while(posx+xmove >= 0 && posx+xmove < m_renderx && posx >= 0 && posx < m_renderx)
    {
        posy = starty;
        while(posy+ymove >= 0 && posy+ymove < m_rendery && posy >= 0 && posy <m_rendery)
        {
            m_result[posx+xmove][posy+ymove] = m_result[posx][posy];
            posy += ystep;
        }
        posx += xstep;
    }
    Complex rangeul = Complex(left,top);
    double xinterval = (right-left)/double(m_renderx);
    double yinterval = (down-top)/double(m_rendery);
    int ystart = 0,yend = m_rendery;
    if(ymove < 0)
    {
        posy = m_rendery-1;
    }else
        posy = 0;
    int ylimit = posy+ymove;
    if(ylimit < posy)
    {
        yend = ylimit;
        int zw = posy;
        posy = ylimit;
        ylimit = zw;
    }else
        ystart = ylimit;
    if(ymove)
    {
        #pragma omp parallel for
        for(uint x = 0;x<m_renderx;x++)
        {
            for(uint y = posy;y<ylimit;y++)
            {
                int res = 0;
                if(!is_julia)
                    res =  calculate_point(Complex(0,0),rangeul+Complex(x*xinterval,y*yinterval));
                else
                    res = calculate_point(rangeul+Complex(x*xinterval,y*yinterval),m_lastpoint);
                if(res == 150)
                {
                    m_result[x][y] = Color(0,0,200);
                }else
                {
                    m_result[x][y] = Color(res*modulator,255-res*modulator,0);
                }
            }
        }
    }
    if(xmove < 0)
    {
        posx = m_renderx-1;
    }else
        posx = 0;
    int xlimit = posx+xmove;
    if(xlimit < posx)
    {
        int zw = posx;
        posx = xlimit;
        xlimit = zw;
    }
    if(xmove)
    {
        #pragma omp parallel for
        for(uint x = posx;x<xlimit;x++)
        {
            for(uint y = ystart;y<yend;y++)
            {
                int res = 0;
                if(!is_julia)
                    res =  calculate_point(Complex(0,0),rangeul+Complex(x*xinterval,y*yinterval));
                else
                    res = calculate_point(rangeul+Complex(x*xinterval,y*yinterval),m_lastpoint);
                if(res == 150)
                {
                    m_result[x][y] = Color(0,0,200);
                }else
                {
                    m_result[x][y] = Color(res*modulator,255-res*modulator,0);
                }
            }
        }
    }
}

void FractalRenderer::render_mandelbrot(double top,double left,double down,double right)
{
    m_lastpoint = Complex(-100,-100);
    Complex rangeul = Complex(left,top);
    double xinterval = (right-left)/double(m_renderx);
    double yinterval = (down-top)/double(m_rendery);
    #pragma omp parallel for
    for(uint x = 0;x<m_renderx;x++)
    {
        for(uint y = 0;y<m_rendery;y++)
        {
            int res = calculate_point(Complex(0,0),rangeul+Complex(x*xinterval,y*yinterval));
            if(res == 150)
            {
                m_result[x][y] = Color(0,0,200);
            }else
            {
                m_result[x][y] = Color(res*modulator,255-res*modulator,0);
            }
        }
    }
}

void FractalRenderer::render_julia(Complex c,double top,double left,double down,double right)
{
    m_lastpoint = c;
    Complex rangeul = Complex(left,top);
    double xinterval = (right-left)/double(m_renderx);
    double yinterval = (down-top)/double(m_rendery);
    #pragma omp parallel for
    for(uint x = 0;x<m_renderx;x++)
    {
        for(uint y = 0;y<m_rendery;y++)
        {
            int res = calculate_point(rangeul+Complex(x*xinterval,y*yinterval),c);
            if(res == 150)
            {
                m_result[x][y] = Color(0,0,200);
            }else
            {
                m_result[x][y] = Color(res*modulator,255-res*modulator,0);
            }
        }
    }
}

void FractalRenderer::render()
{
    render_mandelbrot(-3.0f,2.0f,-2.0f,1.0f);
}

inline int FractalRenderer::calculate_point(Complex start,Complex point) const
{
    Complex akt = start;
    for(uint i = 0;i<150;i++)
    {
        akt = akt*akt+point;
        if(akt.abs() > 1000)
        {
            return i;
        }
    }
    return 150;
}

Complex FractalRenderer::mandelbrot_recursive(Complex z,Complex c) const
{
    return z*z+c;
}

QImage * FractalRenderer::export_picture()
{
    QImage * bild = new QImage(m_renderx, m_rendery, QImage::Format_RGB32);
    for(uint x = 0;x<m_renderx;x++)
    {
        for(uint y = 0;y<m_rendery;y++)
        {
            bild->setPixel(x,y,qRgb(m_result[x][y].r,m_result[x][y].g,m_result[x][y].b));
        }
    }
    return bild;
}

void FractalRenderer::resize(uint newx,uint newy)
{
    if(newx > m_max_x || newy > m_max_y)
    {

        if(newx > m_max_x)
        {
            m_max_x = newx;
        }
        if(newy > m_max_y)
        {
            m_max_y = newy;
        }
        for(uint i = 0;i<m_renderx;i++)
        {
            delete [] m_result[i];
        }
        delete [] m_result;

        m_renderx = newx;
        m_rendery = newy;

        m_result = new Color*[newx];
        for(uint i = 0;i<newx;i++)
        {
            m_result[i] = new Color[newy];
        }
    }else
    {
        m_renderx = newx;
        m_rendery = newy;
    }
}

void FractalRenderer::set_mod(int newmod)
{
    modulator = newmod;
}
