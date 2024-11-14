#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <string>

#include <L_Debug/L_Debug.h>


namespace LST
{

    class File
    {
    private:
        std::string m_path;

    public:
        File();
        File(const File& _other);
        File(File&& _other);
        void operator=(const File& _other);
        void operator=(File&& _other);
        File(const std::string& _path);

    private:
        unsigned int M_get_length(std::ifstream& _file) const;
        std::string M_parse_directory() const;

    public:
        void set_path(const std::string& _path);
        const std::string& get_path() const;

    public:
        std::string extract_block(unsigned int _offset = 0, unsigned int _size = 0xFFFFFFFF) const;

        void clear();
        void append_block(const std::string& _block);

    public:
        bool exists() const;
        unsigned int length() const;

    };

}

#endif // FILE_H
