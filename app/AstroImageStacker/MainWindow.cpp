#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsView>

#include "BatchProcess_Manager.h"
#include "BatchProcess_Visual_GraphicsScene.h"
#include "BatchProcess_PixelMathTask.h"
#include "BatchProcess_NumericSignal.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene(new BatchProcess::Visual::GraphicsScene(this))
    , gv(new QGraphicsView(this))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    BatchProcess::Manager::createInstance(gv);

    ui->mdiArea->addSubWindow(gv);
    gv->setScene(scene);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExecuteTasks_triggered()
{
    BatchProcess::Manager::get()->execute();
}

void MainWindow::on_actionAddTask_triggered()
{
    BatchProcess::PixelMathTask* myTask = new BatchProcess::PixelMathTask(BatchProcess::PixelMathTask::Op_Plus, this);
    myTask->setLhs(
                BatchProcess::SignalPtr(
                    new BatchProcess::ImageSignal(
                        BatchProcess::Signal::Direction_In, "image")));

    myTask->setRhs(
                BatchProcess::SignalPtr(
                    new BatchProcess::NumericSignal(
                        BatchProcess::Signal::Direction_In, "numVal", 5)));

    BatchProcess::TaskPtr task = BatchProcess::TaskPtr(myTask);
    scene->addTask(task);
    BatchProcess::Manager::get()->addTask(task);
}
