#include <Stuff/Bounding_Volume_Hierarchy.h>

#include <Stuff/Math_Stuff.h>
#include <Stuff/Sorter.h>

using namespace LST;


void Bounding_Volume_Hierarchy::Registered_Borders::clear()
{
    m_groups.mark_empty();
    m_total_size = 0;
}

void Bounding_Volume_Hierarchy::Registered_Borders::add(const Borders_Vec& _vec)
{
    m_groups.push({ &_vec, m_total_size });
    m_total_size += _vec.size();
}



Bounding_Volume_Hierarchy::Registered_Borders::Border_Location Bounding_Volume_Hierarchy::Registered_Borders::calculate_border_location(unsigned int _id) const
{
    L_ASSERT(_id < m_total_size);

    for(unsigned int i = m_groups.size() - 1; i > 0; --i)
    {
        unsigned int group_offset = m_groups[i].index_offset;

        if(_id >= group_offset)
            return { i, _id - group_offset };
    }

    return {0, _id};
}


const Border& Bounding_Volume_Hierarchy::Registered_Borders::operator[](unsigned int _id) const
{
    Border_Location location = calculate_border_location(_id);
    return (*m_groups[location.group].borders_vecs)[ location.index ];
}





const Border& Bounding_Volume_Hierarchy::M_get_border(const Context_Internal& _context, unsigned int _id) const
{
    return _context.context.registered_borders[_id];
}


void Bounding_Volume_Hierarchy::M_update_groups_cache(Context_Internal& _context) const
{
    IDs_Vec& cache = _context.groups_cache;

    cache.resize(_context.context.registered_borders.total_size());

    for(unsigned int g_i = 0; g_i < _context.context.registered_borders.groups_amount(); ++g_i)
    {
        for(unsigned int i = 0; i < _context.context.registered_borders.group_size(g_i); ++i)
            cache.push(g_i);
    }
}


Border Bounding_Volume_Hierarchy::M_construct_common_border(const Context_Internal& _context, const IDs_Vec& _ids) const
{
    Border result;

    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        const Border& id_border = M_get_border(_context, _ids[i]);
        result.expand_with(id_border);
    }

    return result;
}

Bounding_Volume_Hierarchy::Separation_Result Bounding_Volume_Hierarchy::M_separate_objects(const Context_Internal& _context, const Border& _border_0, const Border& _border_1, const IDs_Vec& _ids) const
{
    Separation_Result result(_ids.size());

    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        const Border& id_border = M_get_border(_context, _ids[i]);

        bool intersects_with_0 = _border_0.intersects_with(id_border);
        bool intersects_with_1 = _border_1.intersects_with(id_border);

        if(intersects_with_0)
        {
            if(intersects_with_1)
                result.common_ids.push(_ids[i]);
            else
                result.ids_0.push(_ids[i]);
        }
        else if(intersects_with_1)
        {
            result.ids_1.push(_ids[i]);
        }
    }

    return result;
}

bool Bounding_Volume_Hierarchy::M_compare_ids(const IDs_Vec& _ids_0, const IDs_Vec& _ids_1) const
{
    if(_ids_0.size() != _ids_1.size())
        return false;

    for(unsigned int i = 0; i < _ids_0.size(); ++i)
    {
        if(_ids_0[i] != _ids_1[i])
            return false;
    }

    return true;
}


unsigned int Bounding_Volume_Hierarchy::M_find_largest_component(const LST::Border& _rb) const
{
    unsigned int result = 0;
    float max = _rb.extents()[0];

    for(unsigned int i = 1; i < 3; ++i)
    {
        if(_rb.extents()[i] <= max)
            continue;

        result = i;
        max = _rb.extents()[i];
    }

    return result;
}

float Bounding_Volume_Hierarchy::M_calculate_local_separation_point(const Context_Internal& _context, const Border& _border, const IDs_Vec& _ids, unsigned int _separation_axis) const
{
    float average = 0.0f;

    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        const LST::Border& border = M_get_border(_context, _ids[i]);

        average += border.center()[_separation_axis];
    }

    average /= (float)_ids.size();

    return average - _border.center()[_separation_axis];
}

LST::Border Bounding_Volume_Hierarchy::M_split_border(const LST::Border& _border, unsigned int _component, float _local_separation_point, bool _first_half) const
{
    float new_extent_component = (_border.extents()[_component] + ( _first_half ? _local_separation_point : -_local_separation_point )) * 0.5f;

    float new_limit = _border.center()[_component] + _local_separation_point;
    float new_center_component = new_limit + ( _first_half ? -new_extent_component : new_extent_component );

    LST::Border result = _border;

    glm::vec3 new_center = result.center();
    glm::vec3 new_extents = result.extents();

    new_center[_component] = new_center_component;
    new_extents[_component] = new_extent_component;

    result.set_values(new_center, new_extents);

    return result;
}


bool Bounding_Volume_Hierarchy::M_ids_share_group(const Context_Internal& _context, unsigned int _id_0, unsigned int _id_1) const
{
    if(_context.context.registered_borders.groups_amount() < 2)
        return false;

    return _context.groups_cache[_id_0] == _context.groups_cache[_id_1];
}


void Bounding_Volume_Hierarchy::M_append_by_sap(Context_Internal& _context, const IDs_Vec& _ids, unsigned int _separation_axis) const
{
    if(_ids.size() < 2)
        return;

    unsigned int sap_axis = (_separation_axis + 1) % 3;
    unsigned int sap_axis_next = (sap_axis + 1) % 3;

    struct ID_Min
    {
        const LST::Border* border;
        unsigned int id;
        float min;

        inline bool operator<(const ID_Min& _other) const { return min < _other.min; }
    };

    LDS::Vector<ID_Min> projections_vec(_ids.size());
    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        unsigned int id = _ids[i];
        const LST::Border& border = M_get_border(_context, id);
        projections_vec.push({ &border, id, border.calculate_offset(sap_axis) });
    }

    LST::Sorter<ID_Min> sorter;
    sorter.sort(projections_vec);

    for(unsigned int i = 0; i < projections_vec.size() - 1; ++i)
    {
        const ID_Min& current = projections_vec[i];
        float max = current.border->calculate_limit(sap_axis);

        for(unsigned int j = i + 1; j < projections_vec.size(); ++j)
        {
            const ID_Min& check = projections_vec[j];

            if(check.min > max)
                break;

            if(current.border->intersects_with(*check.border, sap_axis_next))
                M_append_pair(_context, current.id, check.id);
        }
    }
}

void Bounding_Volume_Hierarchy::M_process_common_ids(Context_Internal& _context, const Separation_Result& _separation_result, unsigned int _separation_axis) const
{
    if(_separation_result.common_ids.size() < m_min_objects_for_sap)
        M_append_pairs(_context, _separation_result.common_ids);
    else
        M_append_by_sap(_context, _separation_result.common_ids, _separation_axis);

    float common_offset = LST::Math::Max_Float;
    float common_limit = LST::Math::Min_Float;

    for(unsigned int i = 0; i < _separation_result.common_ids.size(); ++i)
    {
        const LST::Border& border = M_get_border(_context, _separation_result.common_ids[i]);

        float component_offset = border.calculate_offset(_separation_axis);
        float component_limit = border.calculate_limit(_separation_axis);

        if(common_offset > component_offset)
            common_offset = component_offset;
        if(common_limit < component_limit)
            common_limit = component_limit;
    }

    for(unsigned int i = 0; i < _separation_result.ids_0.size(); ++i)
    {
        unsigned int current_id = _separation_result.ids_0[i];

        const LST::Border& border = M_get_border(_context, current_id);
        float border_limit = border.calculate_limit(_separation_axis);

        if(border_limit >= common_offset)
            M_append_pairs(_context, _separation_result.common_ids, current_id);
    }

    for(unsigned int i = 0; i < _separation_result.ids_1.size(); ++i)
    {
        unsigned int current_id = _separation_result.ids_1[i];

        const LST::Border& border = M_get_border(_context, current_id);
        float border_offset = border.calculate_offset(_separation_axis);

        if(border_offset <= common_limit)
            M_append_pairs(_context, _separation_result.common_ids, current_id);
    }
}


void Bounding_Volume_Hierarchy::M_append_pairs(Context_Internal& _context, const IDs_Vec& _ids, unsigned int _with_id) const
{
    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        unsigned int id = _ids[i];

        M_append_pair(_context, id, _with_id);
    }
}

void Bounding_Volume_Hierarchy::M_append_pairs(Context_Internal& _context, const IDs_Vec& _ids) const
{
    if(_ids.size() < 2)
        return;

    for(unsigned int i_0 = 0; i_0 < _ids.size() - 1; ++i_0)
    {
        unsigned int id_0 = _ids[i_0];

        for(unsigned int i_1 = i_0 + 1; i_1 < _ids.size(); ++i_1)
        {
            unsigned int id_1 = _ids[i_1];

            M_append_pair(_context, id_0, id_1);
        }
    }
}

void Bounding_Volume_Hierarchy::M_append_pair(Context_Internal& _context, unsigned int _id_0, unsigned int _id_1) const
{
    if(_context.context.filter && !_context.context.filter(_id_0, _id_1))
        return;

    if(M_ids_share_group(_context, _id_0, _id_1))
        return;

    const Border& border_0 = M_get_border(_context, _id_0);
    const Border& border_1 = M_get_border(_context, _id_1);

    if(!border_0.intersects_with(border_1))
        return;

    unsigned int group_0 = _context.groups_cache[_id_0];
    unsigned int group_1 = _context.groups_cache[_id_1];

    unsigned int index_0 = _context.context.registered_borders.calculate_group_index(group_0, _id_0);
    unsigned int index_1 = _context.context.registered_borders.calculate_group_index(group_1, _id_1);

    if(group_0 > group_1)
    {
        std::swap(group_0, group_1);
        std::swap(index_0, index_1);
    }

    _context.result.push({group_0, index_0, group_1, index_1});
}

void Bounding_Volume_Hierarchy::M_find_intersections(Context_Internal& _context, const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const
{
    if(_ids_inside.size() < 2)
        return;

    if(_level >= m_max_depth || _ids_inside.size() <= m_min_objects_in_area)
        return M_append_pairs(_context, _ids_inside);

    unsigned int max_component = M_find_largest_component(_border);
    float local_separation_point = M_calculate_local_separation_point(_context, _border, _ids_inside, max_component);

    LST::Border b_0 = M_split_border(_border, max_component, local_separation_point, true);
    LST::Border b_1 = M_split_border(_border, max_component, local_separation_point, false);

    Separation_Result separation_result = M_separate_objects(_context, b_0, b_1, _ids_inside);

    M_process_common_ids(_context, separation_result, max_component);

    b_0 = M_construct_common_border(_context, separation_result.ids_0);
    b_1 = M_construct_common_border(_context, separation_result.ids_1);

    M_find_intersections(_context, b_0, separation_result.ids_0, _level + 1);
    M_find_intersections(_context, b_1, separation_result.ids_1, _level + 1);
}



Bounding_Volume_Hierarchy::ID_Pairs_Vec Bounding_Volume_Hierarchy::process(const Context& _context) const
{
    if(_context.registered_borders.total_size() < 2)
        return {};

    Context_Internal context(_context);

    M_update_groups_cache(context);

    IDs_Vec initial_ids(_context.registered_borders.total_size());
    for(unsigned int i = 0; i < _context.registered_borders.total_size(); ++i)
        initial_ids.push(i);

    Border initial_border = M_construct_common_border(context, initial_ids);

    M_find_intersections(context, initial_border, initial_ids, 0);

    return context.result;
}
