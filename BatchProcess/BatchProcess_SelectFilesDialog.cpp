#include <BatchProcess/SelectFilesDialog>
#include "ui_BatchProcess_SelectFilesDialog.h"

#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QProgressDialog>
#include <QThreadPool>

#include <AstroBase/PersistentDirInfo>
#include <AstroBase/MessageBox>
#include "BatchProcess_Processor.h"

namespace BatchProcess {

TaskInterface *SelectFilesDialog::getTaskPrototype() const
{
    return taskPrototype;
}

void SelectFilesDialog::setTaskPrototype(TaskInterface *value)
{
    taskPrototype = value;
}

SelectFilesDialog::SelectFilesDialog(QWidget *parent, const QString &filter)
    : QDialog(parent)
    , filter(filter)
    , progressDlg(Q_NULLPTR)
    , ui(new Ui::SelectFilesDialog)
{
    ui->setupUi(this);
    ui->outputDirLineEdit->setText(AstroBase::PersistentDirInfo("BatchProcess?LastOutputDir"));
}

SelectFilesDialog::~SelectFilesDialog()
{
    delete ui;
}

void SelectFilesDialog::onSelectInputFiles()
{
    AstroBase::PersistentDirInfo inputDir("BatchProcess?LastInputDir");

    QStringList inputFiles = QFileDialog::getOpenFileNames(
                this
                , tr("Select input files")
                , inputDir
                , filter);

    if(inputFiles.isEmpty())
        return;

    ui->inputFilesList->clear();


    QFileInfo firstFile(inputFiles[0]);
    inputDir = firstFile.absolutePath();

    foreach(QString fp, inputFiles)
    {
        ui->inputFilesList->addItem(fp);
    }

}

void SelectFilesDialog::incProgress()
{
    if(progressDlg)
    {
        progressDlg->setValue(progressDlg->value()+1);
        if(progressDlg->value() == progressDlg->maximum())
        {
            delete progressDlg;
            progressDlg = Q_NULLPTR;
        }
    }
}

void SelectFilesDialog::onStartBatchProcess()
{
    if(progressDlg)
        delete  progressDlg;

    progressDlg = new QProgressDialog(tr("Batch process ..."), tr("Cancel"), 0, 100, this);
    progressDlg->setModal(true);
    progressDlg->setMinimum(0);
    progressDlg->setMaximum(ui->inputFilesList->count());
    progressDlg->setValue(0);
    progressDlg->show();

    for(int i=0; i<ui->inputFilesList->count(); i++)
    {
        TaskInterfacePtr task(taskPrototype->clone());
        QFileInfo inputFile(ui->inputFilesList->item(i)->text());

        task->setInputFile(inputFile.absoluteFilePath());
        task->setOutputFile(QFileInfo(ui->outputDirLineEdit->text(), inputFile.fileName()).absoluteFilePath());
        Processor* processor = new Processor(task);
        connect(processor, SIGNAL(finished()), this, SLOT(incProgress()));

        QThreadPool::globalInstance()->start(processor);


        if(progressDlg->wasCanceled())
            break;
    }
}

void SelectFilesDialog::onSelectOutputDir()
{
    AstroBase::PersistentDirInfo outputDir("BatchProcess?LastOutputDir");
    QString outPath = outputDir;
    do
    {
        outPath = QFileDialog::getExistingDirectory(this, tr("Select output directory"), outputDir);

        QDir outDir(outPath);
        if(!outPath.isEmpty() && !outDir.exists())
        {
            QMessageBox::critical(this, tr("Select output directory"), tr("Output directory doesn't exist: %0").arg(outputDir));
            continue;
        }

        break;
    }
    while(!outPath.isEmpty());

    if(outPath.isEmpty())
        return;

    ui->outputDirLineEdit->setText(outPath);
    outputDir = outPath;
}

} //namespace BatchProcess
