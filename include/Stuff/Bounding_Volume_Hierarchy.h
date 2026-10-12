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
        using Borders_Vec = LDS::Vector<Border>;

        class Registered_Borders
        {
        public:
            struct Border_Location
            {
                unsigned int group;
                unsigned int index;
            };

        private:
            LDS::Vector<const Borders_Vec*> m_border_vecs;
            unsigned int m_total_size = 0;

        public:
            inline void clear() { m_border_vecs.mark_empty(); m_total_size = 0; }
            inline void add(const Borders_Vec& _vec) { m_border_vecs.push(&_vec); m_total_size += _vec.size(); }

        public:
            Border_Location calculate_border_location(unsigned int _id) const;

            const Border& operator[](unsigned int _id) const;

            inline unsigned int total_size() const { return m_total_size; }
            inline unsigned int groups_amount() const { return m_border_vecs.size(); }

            inline unsigned int calculate_group(unsigned int _id) const { return calculate_border_location(_id).group; }
            inline unsigned int calculate_group_index(unsigned int _id) const { return calculate_border_location(_id).index; }

        };

    private:
        using IDs_Vec = LDS::Vector<unsigned int>;

    private:
        mutable LST::Function<bool(unsigned int, unsigned int)> m_filter;
        mutable Registered_Borders m_registered_borders;
        mutable LDS::Vector<bool> m_exclusions;

        mutable ID_Pairs_Vec m_result;

    private:
        unsigned int m_max_depth = 5;
        unsigned int m_min_objects_in_area = 2;

    public:
        inline void set_max_depth(unsigned int _value) { m_max_depth = _value; }

        inline void set_filter(const Filter_Function& _func) const { m_filter = _func; }
        inline void reset_registered_borders() const { m_registered_borders.clear(); }
        inline void register_borders(const Borders_Vec& _borders) const { m_registered_borders.add(_borders); }

        inline const Registered_Borders& registered_borders() const { return m_registered_borders; }

    private:
        inline const Border& M_get_border(unsigned int _id) const { return m_registered_borders[_id]; }

        unsigned int M_calculate_exclusions_amount(unsigned int _elements_amount) const;
        unsigned int M_calculate_exclusion_index(unsigned int _first_index, unsigned int _second_index) const;
        void M_update_exclusions_cache() const;

        Border M_construct_common_border(const IDs_Vec& _ids) const;
        IDs_Vec M_find_objects_inside(const Border& _border, const IDs_Vec& _ids) const;
        bool M_compare_ids(const IDs_Vec& _ids_0, const IDs_Vec& _ids_1) const;

        unsigned int M_find_largest_component(const Border& _rb) const;
        Border M_split_border(const Border& _border, unsigned int _component, bool _first_half) const;

        bool M_ids_share_group(unsigned int _id_0, unsigned int _id_1) const;

        void M_append_pairs(const IDs_Vec& _ids) const;
        void M_find_intersections(const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const;

    public:
        const ID_Pairs_Vec& process() const;

    };

    using BVH = Bounding_Volume_Hierarchy;

}
