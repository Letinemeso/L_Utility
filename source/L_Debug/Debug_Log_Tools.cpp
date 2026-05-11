#include <L_Debug/Debug_Log_Tools.h>


std::ostream& operator<<(std::ostream& _stream, const glm::vec2& _vec)
{
    _stream << "x: " << _vec.x << " y: " << _vec.y;
    return _stream;
}

std::ostream& operator<<(std::ostream& _stream, const glm::vec3& _vec)
{
    _stream << "x: " << _vec.x << " y: " << _vec.y << " z: " << _vec.z;
    return _stream;
}
