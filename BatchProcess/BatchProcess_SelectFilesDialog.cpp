#include "BatchProcess_SelectFilesDialog.h"
#include "ui_BatchProcess_SelectFilesDialog.h"

#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QProgressDialog>

#include "AstroBase_PersistentDirInfo.h"

namespace BatchProcess {

SelectFilesDialog::SelectFilesDialog(QWidget *parent, const QString &filter)
    : QDialog(parent)
    , ui(new Ui::SelectFilesDialog)
    , filter(filter)
{
    ui->setupUi(this);
    ui->outputDirLineEdit->setText(AstroBase::PersistentDirInfo("BatchProcess?LastOutputDir"));
}

SelectFilesDialog::~SelectFilesDialog()
{
    delete ui;
}

} // namespace BatchProcess

void BatchProcess::SelectFilesDialog::on_selectInputFilesBtn_clicked()
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
    inputDir.setDir(firstFile.absolutePath());

    foreach(QString fp, inputFiles)
    {
        ui->inputFilesList->addItem(fp);
    }
}

void BatchProcess::SelectFilesDialog::on_pushButton_clicked()
{
    QMessageBox mb(this);
    mb.setWindowTitle(tr("Batch process ..."));
    mb.setIcon(QMessageBox::Critical);
    QCheckBox* cb = new QCheckBox(tr("Don't show again"), this);
    cb->setChecked(false);
    mb.setCheckBox(cb);

    bool showInputErrorMsg = true;
    bool showOutputErrorMsg = true;

    QProgressDialog progressDlg(tr("Batch process ..."), tr("Cancel"), 0, ui->inputFilesList->count()-1, this);
    progressDlg.setModal(true);
    progressDlg.show();

    for(int i=0; i<ui->inputFilesList->count(); i++)
    {
        progressDlg.setValue(i);
        if(progressDlg.wasCanceled())
            break;

        cb->setChecked(false);

        QFileInfo inputFileInfo = ui->inputFilesList->item(i)->data(Qt::DisplayRole).toString();
        if(!inputFileInfo.exists())
        {
            if(showInputErrorMsg)
            {
                mb.setText(tr("Input file not found: %0").arg(inputFileInfo.absoluteFilePath()));
                mb.exec();
                showInputErrorMsg = !cb->isChecked();
            }
            continue;
        }

        QFileInfo outputFileInfo = inputFileInfo;
        if(!ui->processInPlaceCb->isChecked())
        {
            outputFileInfo = QFileInfo(QDir(ui->outputDirLineEdit->text()), inputFileInfo.fileName());
        }

        if(!outputFileInfo.absoluteDir().exists())
        {
            if(showOutputErrorMsg)
            {
                mb.setText(tr("Cannot write image file to: %0").arg(outputFileInfo.absoluteFilePath()));
                mb.exec();
                showOutputErrorMsg = !cb->isChecked();
            }
            continue;
        }

        Q_EMIT processFile(inputFileInfo.absoluteFilePath(), outputFileInfo.absoluteFilePath());
    }
}

void BatchProcess::SelectFilesDialog::on_selectOutputDirBtn_clicked()
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
        }

        break;
    }
    while(!outPath.isEmpty());

    if(outPath.isEmpty())
        return;

    ui->outputDirLineEdit->setText(outPath);
    outputDir.setDir(outPath);
}
