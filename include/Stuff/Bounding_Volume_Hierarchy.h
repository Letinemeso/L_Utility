#pragma once

#include <Stuff/Border.h>
#include <Stuff/Function_Wrapper.h>


namespace LST
{

    class Bounding_Volume_Hierarchy final
    {
    public:
        struct ID_Pair
        {
            unsigned int id_0;
            unsigned int id_1;
        };
        using ID_Pairs_Vec = LDS::Vector<ID_Pair>;

        using Filter_Function = LST::Function<bool(unsigned int, unsigned int)>;

    private:
        using IDs_Vec = LDS::Vector<unsigned int>;

    private:
        mutable unsigned int m_total_objects_amount = 0;
        mutable const LDS::Vector<Border>* m_borders = nullptr;
        mutable LDS::Vector<bool> m_exclusions;

        mutable ID_Pairs_Vec m_result;

    private:
        LST::Function<bool(unsigned int, unsigned int)> m_filter;

        unsigned int m_max_depth = 5;
        unsigned int m_min_objects_in_area = 2;

    public:
        inline void set_filter(const Filter_Function& _func) { m_filter = _func; }

        inline void set_max_depth(unsigned int _value) { m_max_depth = _value; }

    private:
        inline const Border& M_get_border(unsigned int _id) const { return (*m_borders)[_id]; }

        unsigned int M_calculate_exclusions_amount(unsigned int _elements_amount) const;
        unsigned int M_calculate_exclusion_index(unsigned int _first_index, unsigned int _second_index) const;
        void M_update_exclusions_cache() const;

        Border M_construct_common_border(const IDs_Vec& _ids) const;
        IDs_Vec M_find_objects_inside(const Border& _border, const IDs_Vec& _ids) const;
        bool M_compare_ids(const IDs_Vec& _ids_0, const IDs_Vec& _ids_1) const;

        unsigned int M_find_largest_component(const Border& _rb) const;
        Border M_split_border(const Border& _border, unsigned int _component, bool _first_half) const;

        void M_append_pairs(const IDs_Vec& _ids) const;
        void M_find_intersections(const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const;

    public:
        const ID_Pairs_Vec& process(const LDS::Vector<Border>& _borders) const;

    };

}
