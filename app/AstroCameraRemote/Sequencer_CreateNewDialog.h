#ifndef SEQUENCER_SEQUENCER_CREATENEWDIALOG_H
#define SEQUENCER_SEQUENCER_CREATENEWDIALOG_H

#include <QDialog>
#include "Sequencer_Protocol.h"

namespace Sequencer {

namespace Ui {
class CreateNewDialog;
}

class CreateNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewDialog(QWidget *parent = nullptr);
    ~CreateNewDialog();

    QString getSubject() const;
    Protocol::Type getType() const;
    Protocol::ColorChannel getColorChannel() const;

private:
    Ui::CreateNewDialog *ui;

protected:
    void accept();

};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_CREATENEWDIALOG_H
