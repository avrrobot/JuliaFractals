#ifndef MOVABLEWINDOW_H
#define MOVABLEWINDOW_H

#include <QWidget>
#include <QPoint>
#include <QLabel>
#include <QString>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QImage>
#include "fractalrenderer.h"

namespace Ui {
class MovableWindow;
}

class MovableWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit MovableWindow(QWidget *parent = 0);
        ~MovableWindow();
        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        void set_julia(bool julia); //either julia (true) or mandelbrot (false)
        void set_left(bool newval);
        QImage *get_image();

    protected:
        void paintEvent(QPaintEvent * event);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent*);
        void resizeEvent(QResizeEvent *);

    public slots:
        void set_julia_number(Complex newnum);
        void mod_changed(int newmod);

    signals:
        void number_chosen(Complex newnum);

    private:
        Ui::MovableWindow *ui;
        FractalRenderer * renderer;
        bool is_julia = false; //default is mandelbrot
        Complex julia_value = Complex(-1,0); //the number used to calculate the Julia Fractal
        int m_movemode = 0;
        int mousex,mousey;
        bool use_left = true;
        QPoint m_oldpoint;
        double top = 2.0f,down = -2.0f,left = -3.0f,right = 1.0f;
        void recal();
};

#endif // MOVABLEWINDOW_H
