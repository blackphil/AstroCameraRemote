#include "Fits_Repair.h"

#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QByteArray>
#include <QRegularExpression>
#include <QMessageBox>
#include <QCheckBox>

#include "Fits_File.h"
#include "BatchProcess_SelectFilesDialog.h"
#include "AstroBase_Exception.h"
#include "AstroBase.h"

namespace Fits {

Repair::Repair(QWidget *parentWidget)
    : QObject(parentWidget)
    , showErrorMsg(true)
    , parentWidget(parentWidget)
{

}

void Repair::startUi()
{
    BatchProcess::SelectFilesDialog dlg(parentWidget);
    connect(&dlg, SIGNAL(processFile(QString, QString)), this, SLOT(processFile(QString,QString)));
    dlg.exec();
}

int Repair::processFile(const QString &inputFilePath, const QString &outputFilePath)
{
    QMessageBox* mb = NULL;

    if(showErrorMsg)
    {
        mb = new QMessageBox(parentWidget);
        connect(mb, SIGNAL(finished(int)), mb, SLOT(deleteLater()));
        mb->setWindowTitle(tr("Error on processing file"));
        mb->setIcon(QMessageBox::Critical);

        QCheckBox* cb = new QCheckBox(tr("Don't show again"), mb);
        cb->setChecked(false);
        mb->setCheckBox(cb);
    }

    try
    {
        File f = File::read(inputFilePath);
        f.write(outputFilePath);
    }
    catch(std::exception& e)
    {
        AB_ERR(e.what());
        if(mb)
        {
            mb->setText(e.what());
            mb->exec();
        }
    }
    catch(...)
    {
        AB_ERR("Unkown error on processing" << inputFilePath);
        if(mb)
        {
            mb->setText(tr("Unkown error on processing file: %0").arg(inputFilePath));
            mb->exec();
        }
    }

    if(mb)
        showErrorMsg = !mb->checkBox()->isChecked();

    return 0;
}

} // namespace Fits
