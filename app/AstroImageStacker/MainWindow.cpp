#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsView>

#include "BatchProcess_Visual_GraphicsScene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsView* gv = new QGraphicsView(this);
    ui->mdiArea->addSubWindow(gv);

    BatchProcess::Visual::GraphicsScene* scene = new BatchProcess::Visual::GraphicsScene(this);
    gv->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}
