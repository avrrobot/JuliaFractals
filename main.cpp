#include "mainwindow.h"
#include <QApplication>
#include <QElapsedTimer>
#include "fractalrenderer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*FractalRenderer * r = new FractalRenderer(1000,1000);
    QElapsedTimer timer;
    timer.start();
    for(int i = 0;i<100;i++)
    {
        r->render();
    }
    cout <<"Es hat: "<<timer.elapsed()<<" ms gedauert!"<<endl;
    return 0;*/
    MainWindow w;
    w.show();

    return a.exec();
}
