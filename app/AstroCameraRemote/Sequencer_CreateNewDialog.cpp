#include "Sequencer_CreateNewDialog.h"
#include "ui_Sequencer_CreateNewDialog.h"

#include <QMessageBox>

namespace Sequencer {

CreateNewDialog::CreateNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewDialog)
{
    ui->setupUi(this);

    for(int i=0; i<Protocol::NumTypes; i++)
    {
        ui->type->addItem(Protocol::typeToString(static_cast<Protocol::Type>(i)));
    }
    ui->type->setCurrentIndex(0);
}

QString CreateNewDialog::getSubject() const
{
    return ui->subject->text();
}

Protocol::Type CreateNewDialog::getType() const
{
    return static_cast<Protocol::Type>(ui->type->currentIndex());
}

void CreateNewDialog::accept()
{
    if(ui->subject->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Create new sequence"), tr("Subject must not be empty!"));
        return;
    }

    QDialog::accept();
}


CreateNewDialog::~CreateNewDialog()
{
    delete ui;
}

} // namespace Sequencer
