#include "BatchProcess_SaveImagesEditDlg.h"
#include "ui_BatchProcess_SaveImagesEditDlg.h"

#include <QFileDialog>

#include "BatchProcess_SaveImagesTask.h"
#include "AstroBase_PersistentDirInfo.h"

namespace BatchProcess {

SaveImagesEditDlg::SaveImagesEditDlg(SaveImagesTask *task, QWidget *parent)
    : QDialog(parent)
    , task(task)
    , ui(new Ui::SaveImagesEditDlg)
{
    ui->setupUi(this);
}

SaveImagesEditDlg::~SaveImagesEditDlg()
{
    delete ui;
}

void SaveImagesEditDlg::on_selectPathBtn_clicked()
{
    AstroBase::PersistentDirInfo dirInfo("BatchProcess?ImagesSaveDir");
    QString path = QFileDialog::getExistingDirectory(this, tr("Select target path"), dirInfo);

    if(path.isEmpty())
        return;

    ui->pathLineEdit->setText(path);

    dirInfo = path;
}

void SaveImagesEditDlg::on_accepted()
{
    task->setTargetPath(ui->pathLineEdit->text());

    if(ui->usePatternRadioBtn->isChecked())
    {
        task->setUsePattern(true);
        task->setTargetFilePattern(ui->pathLineEdit->text());
    }
    else
        task->setUsePattern(false);
}

} // namespace BatchProcess

