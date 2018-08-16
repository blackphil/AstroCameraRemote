#ifndef BATCHPROCESS_BATCHPROCESS_NUMERICVALUEDIALOG_H
#define BATCHPROCESS_BATCHPROCESS_NUMERICVALUEDIALOG_H

#include <QDialog>

namespace BatchProcess {

namespace Ui {
class NumericValueDialog;
}

class NumericValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NumericValueDialog(QWidget *parent = nullptr);
    ~NumericValueDialog();

    double getValue() const;
    void setValue(const double& value);

private:
    Ui::NumericValueDialog *ui;
};


} // namespace BatchProcess
#endif // BATCHPROCESS_BATCHPROCESS_NUMERICVALUEDIALOG_H
