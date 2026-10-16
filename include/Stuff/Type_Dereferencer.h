#pragma once


namespace LST
{

    template <typename _Type>
    struct Type_Dereferencer
    {
        using Type = _Type;
    };

    template <typename _Type>
    struct Type_Dereferencer<_Type&>
    {
        using Type = _Type;
    };
    template <typename _Type>
    struct Type_Dereferencer<const _Type&>
    {
        using Type = _Type;
    };
    template <typename _Type>
    struct Type_Dereferencer<_Type&&>
    {
        using Type = _Type;
    };

}
