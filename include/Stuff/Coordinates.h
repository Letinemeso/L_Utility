#pragma once

#include <L_Debug/L_Debug.h>


namespace LST
{

    class Coordinates
    {
    private:
        unsigned int m_data[3] = { Invalid_Coord, Invalid_Coord, Invalid_Coord };

    public:
        constexpr static unsigned int Invalid_Coord = 0xFFFFFFFF;

        Coordinates() { }
        Coordinates(unsigned int _x, unsigned int _y, unsigned int _z) { x() = _x; y() = _y; z() = _z; }

        inline unsigned int operator[](unsigned int _index) const { L_ASSERT(_index < 3); return m_data[_index]; }
        inline unsigned int& operator[](unsigned int _index) { L_ASSERT(_index < 3); return m_data[_index]; }

        inline unsigned int x() const { return m_data[0]; }
        inline unsigned int& x() { return m_data[0]; }
        inline unsigned int y() const { return m_data[1]; }
        inline unsigned int& y() { return m_data[1]; }
        inline unsigned int z() const { return m_data[2]; }
        inline unsigned int& z() { return m_data[2]; }

        inline bool valid() const { for(unsigned int i = 0; i < 3; ++i) { if(m_data[i] == Invalid_Coord) return false; } return true; }

        inline bool operator==(const Coordinates& _other) const { return x() == _other.x() && y() == _other.y() && z() == _other.z(); }
        inline bool operator!=(const Coordinates& _other) const { return !(*this == _other); }

        bool operator<(const Coordinates& _other) const;
        bool operator>(const Coordinates& _other) const;

        Coordinates operator+(const Coordinates& _other) const;
        Coordinates operator-(const Coordinates& _other) const;

        static Coordinates center(const Coordinates& _first, const Coordinates& _second);

    };

}
