#pragma once

#include <L_Debug/L_Debug.h>


namespace LST
{

    class Signed_Coordinates
    {
    private:
        int m_data[3] = { Invalid_Coord, Invalid_Coord, Invalid_Coord };

    public:
        constexpr static int Invalid_Coord = 0x7FFFFFFF;

        Signed_Coordinates() { }
        Signed_Coordinates(int _x, int _y, int _z) { x() = _x; y() = _y; z() = _z; }

        inline int operator[](int _index) const { L_ASSERT(_index < 3); return m_data[_index]; }
        inline int& operator[](int _index) { L_ASSERT(_index < 3); return m_data[_index]; }

        inline int x() const { return m_data[0]; }
        inline int& x() { return m_data[0]; }
        inline int y() const { return m_data[1]; }
        inline int& y() { return m_data[1]; }
        inline int z() const { return m_data[2]; }
        inline int& z() { return m_data[2]; }

        inline bool valid() const { for(int i = 0; i < 3; ++i) { if(m_data[i] == Invalid_Coord) return false; } return true; }

        inline bool operator==(const Signed_Coordinates& _other) const { return x() == _other.x() && y() == _other.y() && z() == _other.z(); }
        inline bool operator!=(const Signed_Coordinates& _other) const { return !(*this == _other); }

        bool operator<(const Signed_Coordinates& _other) const;
        bool operator>(const Signed_Coordinates& _other) const;

        Signed_Coordinates operator+(const Signed_Coordinates& _other) const;
        Signed_Coordinates operator-(const Signed_Coordinates& _other) const;

        static Signed_Coordinates center(const Signed_Coordinates& _first, const Signed_Coordinates& _second);

    };

}
