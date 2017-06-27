#include <algorithm>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrixmodel.h"
#include "statistics.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(nullptr)
{
    ui->setupUi(this);
    model = new MatrixModel(simulator.getArea());
    ui->flyField->setModel(model);
    ui->flyField->setItemDelegate(new Renderer);

   initInfoWidgets();
   simulator.setParams(ui->field->value(),ui->capacity->value(),ui->stupidity->value());
   model->reload();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initInfoWidgets()
{
    ui->sizeInfo->setValue(ui->field->value());
    ui->capInfo->setValue(ui->capacity->value());
    ui->stupInfo->setValue(ui->stupidity->value());
    ui->stop->setDisabled(true);
    ui->addFly->setDisabled(true);
}

void MainWindow::on_field_valueChanged(int value)
{
    ui->sizeInfo->setValue(value);
    simulator.setParams(ui->field->value(),ui->capacity->value(),ui->stupidity->value());
    model->reload();
}

void MainWindow::on_capacity_valueChanged(int value)
{
    ui->capInfo->setValue(value);
}

void MainWindow::on_stupidity_valueChanged(int value)
{
    ui->stupInfo->setValue(value);
}

void MainWindow::on_addFly_clicked()
{
    simulator.addFly();
}

void MainWindow::setupControls(bool stopped)
{
    ui->stop->setDisabled(stopped);
    ui->addFly->setDisabled(stopped);

    ui->start->setDisabled(!stopped);
    ui->field->setDisabled(!stopped);
    ui->capacity->setDisabled(!stopped);
    ui->stupidity->setDisabled(!stopped);
}

void MainWindow::on_start_clicked()
{
    setupControls(false);

    simulator.setParams(ui->field->value(),ui->capacity->value(),ui->stupidity->value());
    model->reload();
    simulator.start();
}

void MainWindow::on_stop_clicked()
{
    setupControls(true);
    Statistics * st = new Statistics(this);
    st->setWindowTitle("Fly statistics");
    simulator.stop();
    const std::vector<Fly::Ptr_t>& flies = simulator.getFlies();
    QString text;
    std::for_each(flies.begin(), flies.end(), [&text](const Fly::Ptr_t& fly)
    {
        const Telemetry& tel = fly->getTelemetry();
        text.push_back(QString("Age: %1, Distanc: %2, Speed: %3, Status: %4.\n").arg(tel.age).arg(tel.distance).arg(tel.speed).arg(tel.state == Telemetry::Live ? "Live" : "Died"));
    });
    st->setText(text);

    st->show();
}
