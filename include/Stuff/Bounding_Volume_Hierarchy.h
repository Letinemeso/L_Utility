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
            inline unsigned int group_size(unsigned int _group_index) const { return m_border_vecs[_group_index]->size(); }

            inline unsigned int calculate_group(unsigned int _id) const { return calculate_border_location(_id).group; }
            inline unsigned int calculate_group_index(unsigned int _id) const { return calculate_border_location(_id).index; }

        };

        struct Context
        {
            Filter_Function filter;
            Registered_Borders registered_borders;
        };

    private:
        using IDs_Vec = LDS::Vector<unsigned int>;

        struct Context_Internal
        {
            const Context& context;
            LDS::Vector<bool> exclusions;
            IDs_Vec groups_cache;
            ID_Pairs_Vec result;

            Context_Internal(const Context& _context) : context(_context) {  }
        };

    private:
        unsigned int m_max_depth = 3;
        unsigned int m_min_objects_in_area = 2;

    public:
        inline void set_max_depth(unsigned int _value) { m_max_depth = _value; }

    private:
        const Border& M_get_border(const Context_Internal& _context, unsigned int _id) const;

        unsigned int M_calculate_exclusions_amount(unsigned int _elements_amount) const;
        unsigned int M_calculate_exclusion_index(const Context_Internal& _context, unsigned int _first_index, unsigned int _second_index) const;
        void M_update_exclusions(Context_Internal& _context) const;

        void M_update_groups_cache(Context_Internal& _context) const;

        Border M_construct_common_border(const Context_Internal& _context, const IDs_Vec& _ids) const;
        IDs_Vec M_find_objects_inside(const Context_Internal& _context, const Border& _border, const IDs_Vec& _ids) const;
        IDs_Vec M_find_objects_inside(const Context_Internal& _context, const Border& _border) const;
        bool M_compare_ids(const IDs_Vec& _ids_0, const IDs_Vec& _ids_1) const;

        unsigned int M_find_largest_component(const Border& _rb) const;
        Border M_split_border(const Border& _border, unsigned int _component, bool _first_half) const;

        bool M_ids_share_group(const Context_Internal& _context, unsigned int _id_0, unsigned int _id_1) const;

        void M_append_pairs(Context_Internal& _context, const IDs_Vec& _ids) const;
        void M_find_intersections(Context_Internal& _context, const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const;

    public:
        ID_Pairs_Vec process(const Context& _context) const;
        ID_Pairs_Vec process(const Context& _context, const Border& _initial_border) const;

    };

    using BVH = Bounding_Volume_Hierarchy;

}
