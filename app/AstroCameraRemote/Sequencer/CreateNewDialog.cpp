#include "CreateNewDialog.h"
#include "ui_Sequencer_CreateNewDialog.h"

#include <QMessageBox>

namespace Sequencer {

CreateNewDialog::CreateNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sequencer_CreateNewDialog)
{
    ui->setupUi(this);
}

QString CreateNewDialog::getObject() const
{
    return ui->objectName->text();
}

void CreateNewDialog::accept()
{
    if(ui->objectName->text().isEmpty())
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
