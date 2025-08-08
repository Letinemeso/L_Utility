#pragma once

#include <string>


namespace LST
{

    class Substring
    {
    public:
        static constexpr unsigned int Unlimited_Size = 0xFFFFFFFF;

    private:
        std::string& m_raw_string;

        unsigned int m_offset = 0;
        unsigned int m_size = 0;

    public:
        Substring(std::string& _string, unsigned int _offset = 0, unsigned int _size = Unlimited_Size);

        ~Substring();

    public:
        inline std::string& raw_string() { return m_raw_string; }
        inline const std::string& raw_string() const { return m_raw_string; }
        inline unsigned int offset() const { return m_offset; }
        inline unsigned int size() const { return m_size; }

    public:
        void set_parameters(unsigned int _offset = 0, unsigned int _size = Unlimited_Size);
        void move_offset(int _increment);
        void modify_size(int _increment);

    public:
        char& operator[](unsigned int _index);
        char operator[](unsigned int _index) const;

        std::string copy_substring() const;

    };


    class Const_Substring
    {
    public:
        static constexpr unsigned int Unlimited_Size = 0xFFFFFFFF;

    private:
        const std::string& m_raw_string;

        unsigned int m_offset = 0;
        unsigned int m_size = 0;

    public:
        Const_Substring(const std::string& _string, unsigned int _offset = 0, unsigned int _size = Unlimited_Size);

        ~Const_Substring();

    public:
        inline const std::string& raw_string() const { return m_raw_string; }
        inline unsigned int offset() const { return m_offset; }
        inline unsigned int size() const { return m_size; }

    public:
        void set_parameters(unsigned int _offset = 0, unsigned int _size = Unlimited_Size);
        void move_offset(int _increment);
        void modify_size(int _increment);

    public:
        char operator[](unsigned int _index) const;
        bool operator==(const std::string& _what) const;

        std::string copy_substring() const;

    };

}
