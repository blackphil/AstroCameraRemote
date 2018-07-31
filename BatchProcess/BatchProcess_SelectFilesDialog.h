#ifndef BATCHPROCESS_BATCHPROCESS_SELECTFILESDIALOG_H
#define BATCHPROCESS_BATCHPROCESS_SELECTFILESDIALOG_H

#include <QDialog>
#include "batchprocess_global.h"

namespace BatchProcess {

namespace Ui {
class SelectFilesDialog;
}

class BATCHPROCESSSHARED_EXPORT SelectFilesDialog : public QDialog
{
    Q_OBJECT

    QString filter;

Q_SIGNALS :
    void processFile(QString, QString);

public:
    explicit SelectFilesDialog(QWidget *parent = 0, const QString& filter = "");
    ~SelectFilesDialog();


private Q_SLOTS :
    void on_selectInputFilesBtn_clicked();

    void on_pushButton_clicked();

    void on_selectOutputDirBtn_clicked();

private:
    Ui::SelectFilesDialog *ui;
};


} // namespace BatchProcess
#endif // BATCHPROCESS_BATCHPROCESS_SELECTFILESDIALOG_H
