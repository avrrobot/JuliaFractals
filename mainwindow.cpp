#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fractalrenderer.h"

#include <QFileDialog>

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->julia_image->set_julia(true);
    connect(ui->mandelbrot_image,SIGNAL(number_chosen(Complex)),ui->julia_image,SLOT(set_julia_number(Complex)));
    connect(ui->modslider,SIGNAL(valueChanged(int)),ui->julia_image,SLOT(mod_changed(int)));
    connect(ui->modslider,SIGNAL(valueChanged(int)),ui->mandelbrot_image,SLOT(mod_changed(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_berechnen_button_clicked()
{
    /*FractalRenderer rend(500,500);
    rend.render();
    QImage * zw = rend.export_picture();
    QPixmap map = QPixmap::fromImage(*zw);
    ui->julia_image->setPixmap(map);*/
}

void MainWindow::on_mouse_checkbox_toggled(bool checked)
{
    ui->mandelbrot_image->set_left(checked);
}

void MainWindow::on_actionMandelbrot_Bild_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"Select location to save", "", "Image files (*.png *.jpg *.bmp)");
    if(filename.toStdString().length() == 0)
    {
        cout <<"No location selected!"<<endl;
        return;
    }
    QImage * img = ui->mandelbrot_image->get_image();
    img->save(filename);
    delete img;
}

void MainWindow::on_actionJulia_Bild_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"Select location to save", "", "Image files (*.png *.jpg *.bmp)");
    if(filename.toStdString().length() == 0)
    {
        cout <<"No location selected!"<<endl;
        return;
    }
    QImage * img = ui->julia_image->get_image();
    img->save(filename);
    delete img;
}
