#include "mainwindow.h"
#include <QTextStream>
#include "ui_mainwindow.h"
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->newCurve = new QCPCurve(ui->plot1->xAxis, ui->plot1->yAxis);
    this->newCurve->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->plot1->setInteraction(QCP::iRangeDrag, true);
    ui->plot1->setInteraction(QCP::iRangeZoom, true);

    ui->plot2->addGraph();
    ui->plot2->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->plot2->setInteraction(QCP::iRangeDrag, true);
    ui->plot2->setInteraction(QCP::iRangeZoom, true);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(keyLoop()));
    timer->start(50);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPointCurve(double x, double y)
{
    qv1_x.append(x);
    qv1_y.append(y);
}

void MainWindow::addPointGraph(double x, double y)
{
    qv2_x.append(x);
    qv2_y.append(y);
}

void MainWindow::clearData()
{
    qv1_x.clear();
    qv1_y.clear();
    qv2_x.clear();
    qv2_y.clear();
}

void MainWindow::plotCurve()
{
    newCurve->setData(qv1_x, qv1_y);
    if (ui->auto_zoom->isChecked()) {
        zoomFitCurve();
    }
    ui->plot1->replot();
    ui->plot1->update();
}

void MainWindow::plotGraph()
{
    ui->plot2->graph(0)->setData(qv2_x, qv2_y);
    if (ui->auto_zoom->isChecked()) {
        zoomFitGraph();
    }
    ui->plot2->replot();
    ui->plot2->update();
}

void MainWindow::zoomFitCurve()
{
    QTextStream(stdout) << "string to print" << endl;
    //only need to check one, a point needs both a x and a y cord
    if (qv1_x.length() == 0) { //no points (cleared)
        QTextStream(stdout) << "0" << endl;
        ui->plot1->xAxis->setRange(-5.61, 5.65);
        ui->plot1->yAxis->setRange(-5.2, 5.2);
    }else if (qv1_x.length() == 1) { //only one point
        QTextStream(stdout) << "1" << endl;
        ui->plot1->xAxis->setRange(qv1_x.front() -5.1, qv1_x.front() + 5.1);
        ui->plot1->yAxis->setRange(qv1_y.front() -5.1, qv1_y.front() + 5.1);
    }else { //many points
        QTextStream(stdout) << "1+" << endl;
        double x_max = *std::max_element(qv1_x.begin(), qv1_x.end());
        double x_min = *std::min_element(qv1_x.begin(), qv1_x.end());
        double y_max = *std::max_element(qv1_y.begin(), qv1_y.end());
        double y_min = *std::min_element(qv1_y.begin(), qv1_y.end());

        double min = (y_min < x_min) ? y_min : x_min;
        double max = (y_max > x_max) ? y_max : x_max;
        double spacing = (max - min) * 0.1;
        ui->plot1->xAxis->setRange(min - spacing, max + spacing);
        ui->plot1->yAxis->setRange(min - spacing, max + spacing);
    }

}

void MainWindow::zoomFitGraph()
{
    QTextStream(stdout) << "string to print" << endl;
    //only need to check one, a point needs both a x and a y cord
    if (qv2_x.length() == 0) { //no points (cleared)
        QTextStream(stdout) << "0" << endl;
        ui->plot2->xAxis->setRange(-5.61, 5.65);
        ui->plot2->yAxis->setRange(-5.2, 5.2);
    }else if (qv2_x.length() == 1) { //only one point
        QTextStream(stdout) << "1" << endl;
        ui->plot2->xAxis->setRange(qv2_x.front() -5.1, qv2_x.front() + 5.1);
        ui->plot2->yAxis->setRange(qv2_y.front() -5.1, qv2_y.front() + 5.1);
    }else { //many points
        QTextStream(stdout) << "1+" << endl;
        double x_max = *std::max_element(qv2_x.begin(), qv2_x.end());
        double x_min = *std::min_element(qv2_x.begin(), qv2_x.end());
        double y_max = *std::max_element(qv2_y.begin(), qv2_y.end());
        double y_min = *std::min_element(qv2_y.begin(), qv2_y.end());

        double min = (y_min < x_min) ? y_min : x_min;
        double max = (y_max > x_max) ? y_max : x_max;
        double spacing = (max - min) * 0.1;
        ui->plot2->xAxis->setRange(min - spacing, max + spacing);
        ui->plot2->yAxis->setRange(min - spacing, max + spacing);
    }
}

int MainWindow::getThrust()
{
    return ui->thrust->value();
}

int MainWindow::getYaw()
{
    return ui->yaw->value();
}

int MainWindow::getRoll()
{
    return ui->roll->value();
}

int MainWindow::getPitch()
{
    return ui->pitch->value();
}

void MainWindow::keyLoop()
{
    if (keys[0]) {
        ui->yaw->setValue(ui->yaw->value()+1);
    }
    if (keys[1]) {
        ui->roll->setValue(ui->roll->value()-1);
    }
    if (keys[2]) {
        ui->yaw->setValue(ui->yaw->value()-1);
    }
    if (keys[3]) {
        ui->roll->setValue(ui->roll->value()+1);
    }
    if (keys[4]) {
        ui->thrust->setValue(ui->thrust->value()+1);
    }
    if (keys[5]) {
        ui->pitch->setValue(ui->pitch->value()-1);
    }
    if (keys[6]) {
        ui->thrust->setValue(ui->thrust->value()-1);
    }
    if (keys[7]) {
        ui->pitch->setValue(ui->pitch->value()+1);
    }

}

void MainWindow::on_btn_add_clicked()
{
    addPointGraph(ui->bx_x->value(), ui->bx_y->value());
    plotGraph();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QTextStream(stdout) << "key pressed" << endl;

    switch(event->key()){
        case Qt::Key_W: keys[0] = true;
                        break;
        case Qt::Key_A: keys[1] = true;
                        break;
        case Qt::Key_S: keys[2] = true;
                        break;
        case Qt::Key_D: keys[3] = true;
                        break;

        case Qt::Key_Up:keys[4] = true;
                         break;
        case Qt::Key_Left: keys[5] = true;
                           break;
        case Qt::Key_Down: keys[6] = true;
                           break;
        case Qt::Key_Right: keys[7] = true;
                            break;

    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QTextStream(stdout) << "key released" << endl;
    switch(event->key()){
        case Qt::Key_W: keys[0] = false;
                        break;
        case Qt::Key_A: keys[1] = false;
                        break;
        case Qt::Key_S: keys[2] = false;
                        break;
        case Qt::Key_D: keys[3] = false;
                        break;

        case Qt::Key_Up: keys[4] = false;
                         break;
        case Qt::Key_Left: keys[5] = false;
                           break;
        case Qt::Key_Down: keys[6] = false;
                           break;
        case Qt::Key_Right: keys[7] = false;
                            break;

    }
}

void MainWindow::on_btn_clear_clicked()
{
    clearData();
    plotCurve();
    plotGraph();
}
