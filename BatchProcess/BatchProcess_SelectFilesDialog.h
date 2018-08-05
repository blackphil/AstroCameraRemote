#ifndef BATCHPROCESS_BATCHPROCESS_SELECTFILESDIALOG_H
#define BATCHPROCESS_BATCHPROCESS_SELECTFILESDIALOG_H

#include <QDialog>
#include <QProgressDialog>
#include "batchprocess_global.h"
#include "BatchProcess_TaskInterface.h"

namespace BatchProcess {

namespace Ui {
class SelectFilesDialog;
}

class BATCHPROCESSSHARED_EXPORT SelectFilesDialog : public QDialog
{
    Q_OBJECT

    QString filter;
    TaskInterface* taskPrototype;
    QProgressDialog* progressDlg;

Q_SIGNALS :
    void processFile(QString, QString);

public:
    explicit SelectFilesDialog(QWidget *parent = Q_NULLPTR, const QString& filter = "");
    ~SelectFilesDialog();


    TaskInterface *getTaskPrototype() const;
    void setTaskPrototype(TaskInterface *value);

private Q_SLOTS :
    void on_selectInputFilesBtn_clicked();

    void on_pushButton_clicked();

    void on_selectOutputDirBtn_clicked();

    void incProgress();

private:
    Ui::SelectFilesDialog *ui;
};


} // namespace BatchProcess
#endif // BATCHPROCESS_BATCHPROCESS_SELECTFILESDIALOG_H
