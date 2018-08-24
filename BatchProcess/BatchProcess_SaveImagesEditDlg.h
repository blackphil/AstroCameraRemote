#ifndef BATCHPROCESS_BATCHPROCESS_SAVEIMAGESEDITDLG_H
#define BATCHPROCESS_BATCHPROCESS_SAVEIMAGESEDITDLG_H

#include <QDialog>



namespace BatchProcess {

class SaveImagesTask;

namespace Ui {
class SaveImagesEditDlg;
}

class SaveImagesEditDlg : public QDialog
{
    Q_OBJECT

    SaveImagesTask* task;

public:
    explicit SaveImagesEditDlg(SaveImagesTask* task, QWidget *parent = Q_NULLPTR);
    ~SaveImagesEditDlg();

private slots:
    void on_selectPathBtn_clicked();

    void on_accepted();

private:
    Ui::SaveImagesEditDlg *ui;
};


} // namespace BatchProcess
#endif // BATCHPROCESS_BATCHPROCESS_SAVEIMAGESEDITDLG_H
