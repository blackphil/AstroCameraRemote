#ifndef ASTROBASE_MESSAGEBOX_H
#define ASTROBASE_MESSAGEBOX_H

#include <QMessageBox>

namespace AstroBase {

class  MessageBox : public QMessageBox
{
    Q_OBJECT

public :
    MessageBox(QWidget* parent = Q_NULLPTR);

public:
    static MessageBox* createCritical(
            QWidget* parent
            , const QString& title
            , bool showAgainCheckBox = true);

    void showAgain(const QString& msg);
};

} // namespace AstroBase

#endif // ASTROBASE_MESSAGEBOX_H
