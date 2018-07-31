#include "AstroBase_Exception.h"

namespace AstroBase {

Exception::Exception(const QString &msg)
    : std::runtime_error(msg.toStdString())
{

}

} // namespace AstroBase
