#ifndef SEQUENCER_SEQUENCER_CREATENEWDIALOG_H
#define SEQUENCER_SEQUENCER_CREATENEWDIALOG_H

#include <QDialog>


namespace Ui {
class Sequencer_CreateNewDialog;
}

namespace Sequencer {

class CreateNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewDialog(QWidget *parent = nullptr);
    ~CreateNewDialog();

    QString getObject() const;

private:
    ::Ui::Sequencer_CreateNewDialog *ui;

protected:
    void accept();

};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_CREATENEWDIALOG_H
