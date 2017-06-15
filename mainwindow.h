#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_berechnen_button_clicked();
        void on_mouse_checkbox_toggled(bool checked);
        void on_actionJulia_Bild_triggered();
        void on_actionMandelbrot_Bild_triggered();

private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
