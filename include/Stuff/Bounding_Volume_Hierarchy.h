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
            unsigned int group_id_0;
            unsigned int id_0;

            unsigned int group_id_1;
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

            struct Group_Data
            {
                const Borders_Vec* borders_vecs;
                unsigned int index_offset;
            };

        private:
            LDS::Vector<Group_Data> m_groups;
            unsigned int m_total_size = 0;

        public:
            void clear();
            void add(const Borders_Vec& _vec);

        public:
            Border_Location calculate_border_location(unsigned int _id) const;

            const Border& operator[](unsigned int _id) const;

            inline unsigned int total_size() const { return m_total_size; }
            inline unsigned int groups_amount() const { return m_groups.size(); }
            inline unsigned int group_size(unsigned int _group_index) const { return m_groups[_group_index].borders_vecs->size(); }

            inline unsigned int calculate_group(unsigned int _id) const { return calculate_border_location(_id).group; }
            inline unsigned int calculate_group_index(unsigned int _id) const { return calculate_border_location(_id).index; }
            inline unsigned int calculate_group_index(unsigned int _group_index, unsigned int _id) const { return _id - m_groups[_group_index].index_offset; }

        };

        struct Context
        {
            Filter_Function filter;
            Registered_Borders registered_borders;
        };

    private:
        using IDs_Vec = LDS::Vector<unsigned int>;

        struct Separation_Result
        {
            IDs_Vec ids_0;
            IDs_Vec ids_1;
            IDs_Vec common_ids;

            Separation_Result(unsigned int _size)
                : ids_0(_size), ids_1(_size), common_ids(_size) { }

            Separation_Result(Separation_Result&& _from)
            : ids_0(_from.ids_0), ids_1(_from.ids_1), common_ids(_from.common_ids) { }
        };

        struct Context_Internal
        {
            const Context& context;
            IDs_Vec groups_cache;
            ID_Pairs_Vec result;

            Context_Internal(const Context& _context) : context(_context) {  }
        };

    private:
        unsigned int m_max_depth = 3;
        unsigned int m_min_objects_in_area = 2;
        unsigned int m_min_objects_for_sap = 16;    //  sap - sweep and prune

    public:
        inline void set_max_depth(unsigned int _value) { m_max_depth = _value; }

    private:
        const Border& M_get_border(const Context_Internal& _context, unsigned int _id) const;

        void M_update_groups_cache(Context_Internal& _context) const;

        Border M_construct_common_border(const Context_Internal& _context, const IDs_Vec& _ids) const;
        Separation_Result M_separate_objects(const Context_Internal& _context, const Border& _border_0, const Border& _border_1, const IDs_Vec& _ids) const;
        bool M_compare_ids(const IDs_Vec& _ids_0, const IDs_Vec& _ids_1) const;

        unsigned int M_find_largest_component(const Border& _rb) const;
        float M_calculate_local_separation_point(const Context_Internal& _context, const Border& _border, const IDs_Vec& _ids, unsigned int _separation_axis) const;
        Border M_split_border(const Border& _border, unsigned int _component, float _local_separation_point, bool _first_half) const;

        bool M_ids_share_group(const Context_Internal& _context, unsigned int _id_0, unsigned int _id_1) const;

        void M_append_by_sap(Context_Internal& _context, const IDs_Vec& _ids, unsigned int _separation_axis) const;
        void M_process_common_ids(Context_Internal& _context, const Separation_Result& _separation_result, unsigned int _separation_axis) const;

        void M_append_pairs(Context_Internal& _context, const IDs_Vec& _ids, unsigned int _with_id) const;
        void M_append_pairs(Context_Internal& _context, const IDs_Vec& _ids) const;
        void M_append_pair(Context_Internal& _context, unsigned int _id_0, unsigned int _id_1) const;

        void M_find_intersections(Context_Internal& _context, const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const;

    public:
        ID_Pairs_Vec process(const Context& _context) const;

    };

    using BVH = Bounding_Volume_Hierarchy;

}
