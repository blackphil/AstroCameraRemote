#ifndef MAINWINDOWWINDOW_H
#define MAINWINDOWWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

#include "BatchProcess_Task.h"
#include "BatchProcess_Visual_GraphicsScene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    BatchProcess::Visual::GraphicsScene* scene;
    QGraphicsView* gv;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionExecuteTasks_triggered();

    void on_actionSave_images_triggered();

    void on_actionPlus_Operator_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOWWINDOW_H
