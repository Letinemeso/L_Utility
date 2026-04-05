#pragma once


namespace LST
{

    class UInt128 final
    {
    public:
        using Part_Type = long long unsigned int;

    private:
        Part_Type m_raw[2];

    public:
        UInt128();
        UInt128(Part_Type _upper_part, Part_Type _lower_part);
        UInt128(const UInt128& _other);
        ~UInt128();

        template<typename _Type>
        UInt128(const _Type* _upper_part, const _Type* _lower_part);

        void operator=(const UInt128& _other);

        void from_uint(Part_Type _upper_part, Part_Type _lower_part);

    public:
        inline Part_Type lower_part() const { return m_raw[0]; }
        inline Part_Type upper_part() const { return m_raw[1]; }

    public:
        bool operator==(const UInt128& _other) const;
        bool operator<(const UInt128& _other) const;
        bool operator>(const UInt128& _other) const;
        bool operator<=(const UInt128& _other) const;
        bool operator>=(const UInt128& _other) const;

    };


    template<typename _Type>
    UInt128::UInt128(const _Type* _upper_part, const _Type* _lower_part)
    {
        from_uint((Part_Type)_upper_part, (Part_Type)_lower_part);
    }

}
