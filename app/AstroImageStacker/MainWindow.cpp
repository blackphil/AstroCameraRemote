#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsView>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "BatchProcess_Manager.h"
#include "BatchProcess_Visual_GraphicsScene.h"
#include "BatchProcess_PixelMathTask.h"
#include "BatchProcess_NumericSignal.h"
#include "BatchProcess_SaveImagesTask.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene(new BatchProcess::Visual::GraphicsScene(this))
    , gv(new QGraphicsView(this))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    BatchProcess::Manager::createInstance(gv);

    QMdiSubWindow* subWin = ui->mdiArea->addSubWindow(gv);
    subWin->showMaximized();
    gv->setScene(scene);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExecuteTasks_triggered()
{
    try
    {
        BatchProcess::Manager::get()->execute();
        QMessageBox::information(this, tr("Execute tasks"), tr("Finished"));
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(this, tr("Execute tasks"), tr("Error: %0").arg(e.what()));
    }
}

void MainWindow::on_actionSave_images_triggered()
{
    BatchProcess::SaveImagesTask* myTask = new BatchProcess::SaveImagesTask(this);
    BatchProcess::TaskPtr task = BatchProcess::TaskPtr(myTask);
    scene->addTask(task);
    BatchProcess::Manager::get()->addTask(task);
}

void MainWindow::on_actionPlus_Operator_triggered()
{
    BatchProcess::PixelMathTask* myTask = new BatchProcess::PixelMathTask(
                BatchProcess::PixelMathTask::Op_Plus
                , BatchProcess::PixelMathTask::Summand_Image
                , BatchProcess::PixelMathTask::Summand_Numeric
                , this);

    BatchProcess::TaskPtr task = BatchProcess::TaskPtr(myTask);
    scene->addTask(task);
    BatchProcess::Manager::get()->addTask(task);

}
