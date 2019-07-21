#include "Sequencer_CreateNewDialog.h"
#include "ui_Sequencer_CreateNewDialog.h"

#include <QMessageBox>

namespace Sequencer {

CreateNewDialog::CreateNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewDialog)
{
    ui->setupUi(this);


    for(auto t : PhotoShot::AllTypes)
    {
        ui->type->addItem(PhotoShot::typeToString(t));
    }
    ui->type->setCurrentIndex(0);
}

QString CreateNewDialog::getSubject() const
{
    return ui->objectName->text();
}

PhotoShot::Type CreateNewDialog::getType() const
{
    return static_cast<PhotoShot::Type>(ui->type->currentIndex());
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
