#include "movablewindow.h"
#include "ui_movablewindow.h"

MovableWindow::MovableWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MovableWindow)
{
    ui->setupUi(this);
    //setMouseTracking(true);
    renderer = new FractalRenderer(width(),height());

    recal();
}

MovableWindow::~MovableWindow()
{
    delete ui;
}

QSize MovableWindow::minimumSizeHint() const
{
    return QSize(50,50);
}

QSize MovableWindow::sizeHint() const
{
    return QSize(500,500);
}

void MovableWindow::paintEvent(QPaintEvent *)
{

}

void MovableWindow::recal()
{
    if(is_julia)
    {
        renderer->render_julia(julia_value,top,left,down,right);
    }else
    {
        renderer->render_mandelbrot(top,left,down,right);
    }
    QImage * zw = renderer->export_picture();
    QPixmap map = QPixmap::fromImage(*zw);
    if(is_julia)
    {
        QPainter *painter = new QPainter(&map);
        QFont font("", 12); // arbitrary (default) font, size 2
        painter->setFont(font);
        painter->drawText(QPointF(30,30), QString("(%1, %2)").arg(julia_value.r).arg(julia_value.i));
        painter->end();
    }
    ui->m_picture->setPixmap(map);
    delete zw;
}

QImage * MovableWindow::get_image()
{
    return renderer->export_picture();
}

void MovableWindow::mousePressEvent(QMouseEvent * event)
{
    if((event->button() == Qt::LeftButton && use_left) || (event->button() == Qt::RightButton && !use_left))
    {
        m_movemode = 1;
        m_oldpoint = event->pos();
    }else if((event->button() == Qt::RightButton && use_left) || (event->button() == Qt::LeftButton && !use_left))
    {
        m_movemode = 2;
        emit number_chosen(Complex(left+(right-left)*double(event->pos().x())/double(width()),
                                  down+(top-down)*double(height()-event->pos().y())/double(height())));
    }
}

void MovableWindow::mouseMoveEvent(QMouseEvent * event)
{
    if(m_movemode == 1)
    {
        QPoint newpos = event->pos();
        QPoint dif = newpos-m_oldpoint;
        left -= (right-left)*double(dif.x())/double(width());
        right -= (right-left)*double(dif.x())/double(width());
        top += (top-down)*double(dif.y())/double(height());
        down += (top-down)*double(dif.y())/double(height());
        m_oldpoint = newpos;
        //recal();
        renderer->move_window(dif.x(),dif.y(),top,left,down,right);
        QImage * zw = renderer->export_picture();
        QPixmap map = QPixmap::fromImage(*zw);
        ui->m_picture->setPixmap(map);
        delete zw;
    }else if(m_movemode == 2)
    {
        mousex = event->pos().x();
        mousey = event->pos().y();
        emit number_chosen(Complex(left+(right-left)*double(event->pos().x())/double(width()),
                                  down+(top-down)*double(height()-event->pos().y())/double(height())));
    }
}

void MovableWindow::wheelEvent(QWheelEvent* event)
{
    QPoint scr = event->angleDelta();
    if(!scr.isNull())
    {
        double mult = scr.y() > 0 ? (100.0f/scr.y()) : 1/(-100.0f/scr.y());
        double zwx = left+double((right-left)*event->x())/double(width());
        double zwy = down+double((top-down)*(height()-event->y()))/double(height());
        double nw = (right-left)*mult;
        double nh = (top-down)*mult;
        left = zwx-(nw*(zwx-left)/(right-left));
        right = left+nw;
        down = zwy-(nh*(zwy-down)/(top-down));
        top = down+nh;
        recal();
    }
}

void MovableWindow::mouseReleaseEvent(QMouseEvent * event)
{
    m_movemode = 0;
}

void MovableWindow::resizeEvent(QResizeEvent * event)
{
    QSize nsize = event->size();
    renderer->resize(nsize.width(),nsize.height());
    recal();
}

void MovableWindow::set_julia(bool julia)
{
    is_julia = julia;
    recal();
}

void MovableWindow::set_julia_number(Complex newnum)
{
    julia_value = newnum;
    recal();
}

void MovableWindow::mod_changed(int newmod)
{
    renderer->set_mod(newmod);
    recal();
}

void MovableWindow::set_left(bool newval)
{
    use_left = newval;
}
