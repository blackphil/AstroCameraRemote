#include "AstroBase_MessageBox.h"

#include <QCheckBox>

namespace AstroBase {

MessageBox::MessageBox(QWidget *parent)
    : QMessageBox(parent)
{
}

MessageBox* MessageBox::createCritical(
        QWidget *parent
        , const QString &title
        , bool showAgainCheckBox)
{
    MessageBox* mb(new MessageBox(parent));
    mb->setWindowTitle(title);
    if(showAgainCheckBox)
    {
        mb->setCheckBox(new QCheckBox(tr("Don't show again"), mb));
        mb->checkBox()->setChecked(false);
    }
    return mb;
}

void MessageBox::showAgain(const QString &msg)
{
    setText(msg);
    if(!checkBox() || !checkBox()->isChecked())
        exec();
}

} // namespace AstroBase
