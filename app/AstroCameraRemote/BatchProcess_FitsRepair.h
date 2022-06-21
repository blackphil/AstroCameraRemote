#ifndef BATCHPROCESS_FITSREPAIR_H
#define BATCHPROCESS_FITSREPAIR_H

#include <BatchProcess/TaskInterface>
#include <QDir>

namespace BatchProcess {

class FitsRepair : public TaskInterface
{
    QString input;
    QString output;

public:
    FitsRepair();

    void setInputFile(const QString& file);
    void setOutputFile(const QString& file);
    bool process();
    TaskInterface* clone() const;
};

} // namespace BatchProcess

#endif // BATCHPROCESS_FITSREPAIR_H
