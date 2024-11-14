#include <Stuff/File.h>

#include <filesystem>

using namespace LST;


File::File()
{

}

File::File(const File& _other)
    : m_path(_other.m_path)
{

}

File::File(File&& _other)
    : m_path((std::string&&)_other.m_path)
{

}

void File::operator=(const File& _other)
{
    m_path = _other.m_path;
}

void File::operator=(File&& _other)
{
    m_path = (std::string&&)_other.m_path;
}

File::File(const std::string& _path)
    : m_path(_path)
{

}



unsigned int File::M_get_length(std::ifstream &_file) const
{
    if(!_file.is_open())
        return 0;

    unsigned int position = _file.tellg();

    _file.seekg(0, std::fstream::end);
    unsigned int result = _file.tellg();
    _file.seekg(position, std::fstream::beg);

    return result;
}

std::string File::M_parse_directory() const
{
    if(m_path.size() == 0)
        return "";

    unsigned int slash_position = m_path.size() - 1;
    for(; slash_position > 0; --slash_position)
    {
        if(m_path[slash_position] == '/')
            break;
    }

    if(slash_position == 0)
        return "";

    return m_path.substr(0, slash_position + 1);
}



void File::set_path(const std::string& _path)
{
    m_path = _path;
}

const std::string& File::get_path() const
{
    return m_path;
}



std::string File::extract_block(unsigned int _offset, unsigned int _size) const
{
    std::string result;

    std::ifstream file(m_path, std::ifstream::binary);

    L_ASSERT(file.is_open());

    unsigned int length = M_get_length(file);

    if(length <= _offset)
        return result;

    if(_size > length)
        _size = length - _offset;

    file.seekg(_offset, std::ifstream::beg);

    result.resize(_size);
    char* buffer = (char*)result.c_str();
    file.read(buffer, _size);

    file.close();

    return result;
}


void File::clear()
{
    std::ofstream file(m_path, std::ofstream::trunc);
    if(file.is_open())
        file.close();
}

void File::append_block(const std::string& _block)
{
    if(!exists())
        std::filesystem::create_directories(M_parse_directory());

    std::ofstream file(m_path, std::ofstream::binary | std::ofstream::app);
    if(!file.is_open())
        return;

    file << _block;

    file.close();
}



bool File::exists() const
{
    std::ifstream file(m_path, std::ifstream::binary);
    if(!file.is_open())
        return false;
    file.close();
    return true;
}

unsigned int File::length() const
{
    std::ifstream file(m_path, std::ifstream::binary);

    unsigned int length = M_get_length(file);

    file.close();

    return length;
}
