#include <AstroBase/Exception>

namespace AstroBase {

Exception::Exception(const QString &msg) noexcept
    : std::runtime_error(msg.toStdString())
{

}

Exception::Exception(const Exception& rhs) noexcept
    : std::runtime_error(rhs)
{}

Exception& Exception::operator=(const Exception& rhs) noexcept
{
    std::runtime_error::operator=(rhs);
    return *this;
}

} // namespace AstroBase
