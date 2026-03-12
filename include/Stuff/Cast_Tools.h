#pragma once

namespace LST
{

    template<typename _Cast_To, typename _Cast_From>
    inline _Cast_To* raw_cast(_Cast_From* _what)
    {
        return (_Cast_To*)_what;
    }

    template<typename _Cast_To, typename _Cast_From>
    inline const _Cast_To* raw_cast(const _Cast_From* _what)
    {
        return (const _Cast_To*)_what;
    }

    template<typename _Cast_To, typename _Cast_From>
    inline _Cast_To& raw_cast(_Cast_From& _what)
    {
        return (_Cast_To&)_what;
    }

    template<typename _Cast_To, typename _Cast_From>
    inline const _Cast_To& raw_cast(const _Cast_From& _what)
    {
        return (const _Cast_To&)_what;
    }

    template<typename _Cast_To, typename _Cast_From>
    const _Cast_To& raw_cast(_Cast_From&& _what) = delete;      //  deleting this overload to avoid casting temporary objects into reference variables


    template<typename _Type>
    inline _Type&& move(_Type& _what)
    {
        return (_Type&&)_what;
    }

}
