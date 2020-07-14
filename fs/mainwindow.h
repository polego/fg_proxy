#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addPointCurve(double x, double y);
    void addPointGraph(double x, double y);
    void clearData();
    void plotCurve();
    void plotGraph();
    void zoomFitCurve();
    void zoomFitGraph();
    int getThrust();
    int getYaw();
    int getRoll();
    int getPitch(); 

private slots:
    void keyLoop();
    void on_btn_add_clicked(); 
    void on_btn_clear_clicked();

private:
    bool keys[8];
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    Ui::MainWindow *ui;
    QVector<double> qv1_x, qv1_y, qv2_x, qv2_y;
    QCPCurve *newCurve;

};
#endif // MAINWINDOW_H
