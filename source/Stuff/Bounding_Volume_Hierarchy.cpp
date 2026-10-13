#include <Stuff/Bounding_Volume_Hierarchy.h>

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

unsigned int Bounding_Volume_Hierarchy::M_calculate_exclusion_index(const Context_Internal& _context, unsigned int _first_index, unsigned int _second_index) const
{
    L_ASSERT(_first_index != _second_index);

    if(_first_index > _second_index)
        std::swap(_first_index, _second_index);

    return (_first_index * _context.context.registered_borders.total_size()) - M_calculate_exclusions_amount(_first_index + 1) + (_second_index - _first_index - 1);
}

void Bounding_Volume_Hierarchy::M_update_exclusions(Context_Internal& _context) const
{
    unsigned int exclusions_size = M_calculate_exclusions_amount(_context.context.registered_borders.total_size());
    if(exclusions_size == 0)
        return;

    _context.exclusions.resize(exclusions_size);
    _context.exclusions.fill_raw(0);
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

Bounding_Volume_Hierarchy::IDs_Vec Bounding_Volume_Hierarchy::M_find_objects_inside(const Context_Internal& _context, const Border& _border, const IDs_Vec& _ids) const
{
    IDs_Vec result(_ids.size());

    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        const Border& id_border = M_get_border(_context, _ids[i]);

        if(_border.intersects_with(id_border))
            result.push(_ids[i]);
    }

    return result;
}

Bounding_Volume_Hierarchy::IDs_Vec Bounding_Volume_Hierarchy::M_find_objects_inside(const Context_Internal& _context, const Border& _border) const
{
    IDs_Vec result(_context.context.registered_borders.total_size());

    for(unsigned int i = 0; i < _context.context.registered_borders.total_size(); ++i)
    {
        const Border& id_border = M_get_border(_context, i);

        if(_border.intersects_with(id_border))
            result.push(i);
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

LST::Border Bounding_Volume_Hierarchy::M_split_border(const LST::Border& _border, unsigned int _component, bool _first_half) const
{
    float extent_halved = _border.extents()[_component] * 0.5f;

    LST::Border result = _border;

    glm::vec3 new_center = result.center();
    glm::vec3 new_extents = result.extents();

    new_center[_component] += (_first_half ? -extent_halved : extent_halved);
    new_extents[_component] = extent_halved;

    result.set_values(new_center, new_extents);

    return result;
}


bool Bounding_Volume_Hierarchy::M_ids_share_group(const Context_Internal& _context, unsigned int _id_0, unsigned int _id_1) const
{
    if(_context.context.registered_borders.groups_amount() < 2)
        return false;

    return _context.groups_cache[_id_0] == _context.groups_cache[_id_1];
}


void Bounding_Volume_Hierarchy::M_append_pairs(Context_Internal& _context, const IDs_Vec& _ids) const
{
    for(unsigned int i_0 = 0; i_0 < _ids.size() - 1; ++i_0)
    {
        unsigned int id_0 = _ids[i_0];

        for(unsigned int i_1 = i_0 + 1; i_1 < _ids.size(); ++i_1)
        {
            unsigned int id_1 = _ids[i_1];

            unsigned int exclusion_index = M_calculate_exclusion_index(_context, id_0, id_1);
            if(_context.exclusions[exclusion_index])
                continue;
            _context.exclusions[exclusion_index] = true;

            if(_context.context.filter && !_context.context.filter(id_0, id_1))
                continue;

            if(M_ids_share_group(_context, id_0, id_1))
                continue;

            const Border& border_0 = M_get_border(_context, id_0);
            const Border& border_1 = M_get_border(_context, id_1);

            if(!border_0.intersects_with(border_1))
                continue;

            unsigned int group_0 = _context.groups_cache[id_0];
            unsigned int group_1 = _context.groups_cache[id_1];

            unsigned int index_0 = _context.context.registered_borders.calculate_group_index(group_0, id_0);
            unsigned int index_1 = _context.context.registered_borders.calculate_group_index(group_1, id_1);

            if(group_0 > group_1)
            {
                std::swap(group_0, group_1);
                std::swap(index_0, index_1);
            }

            _context.result.push({group_0, index_0, group_1, index_1});
        }
    }
}

void Bounding_Volume_Hierarchy::M_find_intersections(Context_Internal& _context, const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const
{
    if(_ids_inside.size() < 2)
        return;

    if(_level >= m_max_depth || _ids_inside.size() <= m_min_objects_in_area)
        return M_append_pairs(_context, _ids_inside);

    unsigned int max_component = M_find_largest_component(_border);
    LST::Border b_0 = M_split_border(_border, max_component, true);
    LST::Border b_1 = M_split_border(_border, max_component, false);

    IDs_Vec ids_0 = M_find_objects_inside(_context, b_0, _ids_inside);
    IDs_Vec ids_1 = M_find_objects_inside(_context, b_1, _ids_inside);

    if(M_compare_ids(ids_0, ids_1))
    {
        b_0 = M_construct_common_border(_context, ids_0);

        M_find_intersections(_context, b_0, ids_0, _level + 1);
    }
    else
    {
        b_0 = M_construct_common_border(_context, ids_0);
        b_1 = M_construct_common_border(_context, ids_1);

        M_find_intersections(_context, b_0, ids_0, _level + 1);
        M_find_intersections(_context, b_1, ids_1, _level + 1);
    }
}



Bounding_Volume_Hierarchy::ID_Pairs_Vec Bounding_Volume_Hierarchy::process(const Context& _context) const
{
    if(_context.registered_borders.total_size() < 2)
        return {};

    Context_Internal context(_context);

    M_update_exclusions(context);
    M_update_groups_cache(context);

    IDs_Vec initial_ids(_context.registered_borders.total_size());
    for(unsigned int i = 0; i < _context.registered_borders.total_size(); ++i)
        initial_ids.push(i);

    Border initial_border = M_construct_common_border(context, initial_ids);

    M_find_intersections(context, initial_border, initial_ids, 0);

    return context.result;
}

Bounding_Volume_Hierarchy::ID_Pairs_Vec Bounding_Volume_Hierarchy::process(const Context& _context, const Border& _initial_border) const
{
    if(_context.registered_borders.total_size() < 2)
        return {};

    Context_Internal context(_context);

    M_update_exclusions(context);
    M_update_groups_cache(context);

    IDs_Vec initial_ids = M_find_objects_inside(_context, _initial_border);

    M_find_intersections(context, _initial_border, initial_ids, 0);

    return context.result;
}
