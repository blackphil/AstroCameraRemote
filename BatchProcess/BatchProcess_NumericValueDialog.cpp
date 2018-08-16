#include "BatchProcess_NumericValueDialog.h"
#include "ui_BatchProcess_NumericValueDialog.h"

namespace BatchProcess {

NumericValueDialog::NumericValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumericValueDialog)
{
    ui->setupUi(this);
}

NumericValueDialog::~NumericValueDialog()
{
    delete ui;
}

double NumericValueDialog::getValue() const
{
    return ui->doubleSpinBox->value();
}

void NumericValueDialog::setValue(const double& value)
{
    ui->doubleSpinBox->setValue(value);
}

} // namespace BatchProcess
