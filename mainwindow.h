#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "flysimulator.h"

namespace Ui {
class MainWindow;
}

class MatrixModel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_field_valueChanged(int value);
    void on_capacity_valueChanged(int value);
    void on_stupidity_valueChanged(int value);

    void on_addFly_clicked();
    void on_start_clicked();
    void on_stop_clicked();

private:
    void setupControls(bool stopped);
    void initInfoWidgets();

    Ui::MainWindow *ui;

    MatrixModel * model;
    FlySimulator simulator;
};

#endif // MAINWINDOW_H
