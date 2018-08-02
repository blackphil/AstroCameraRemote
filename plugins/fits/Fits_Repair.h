#ifndef FITS_REPAIR_H
#define FITS_REPAIR_H

#include "fits_global.h"
#include <QObject>

class QWidget;

namespace Fits {

class Repair : public QObject
{
    Q_OBJECT

    bool showErrorMsg;
    QWidget* parentWidget;

public:
    Repair(QWidget* parentWidget);

    void startUi();


private Q_SLOTS :
    int processFile(const QString& inputFilePath, const QString& outputFilePath);
};

} // namespace Fits

#endif // FITS_REPAIR_H
