#include <Stuff/Bounding_Volume_Hierarchy.h>

using namespace LST;


Bounding_Volume_Hierarchy::Registered_Borders::Border_Location Bounding_Volume_Hierarchy::Registered_Borders::calculate_border_location(unsigned int _id) const
{
    unsigned int counter = 0;
    for(unsigned int i = 0; i < m_border_vecs.size(); ++i)
    {
        const Borders_Vec& borders_vec = *m_border_vecs[i];

        unsigned int local_id = _id - counter;
        if(local_id < borders_vec.size())
            return { i, local_id };

        counter += m_border_vecs[i]->size();
    }

    L_ASSERT(false);
    return {};
}


const Border& Bounding_Volume_Hierarchy::Registered_Borders::operator[](unsigned int _id) const
{
    Border_Location location = calculate_border_location(_id);
    return (*m_border_vecs[location.group])[ location.index ];
}





unsigned int Bounding_Volume_Hierarchy::M_calculate_exclusions_amount(unsigned int _elements_amount) const
{
    if(_elements_amount < 2)
        return 0;

    return ( _elements_amount * ( _elements_amount - 1 ) ) / 2;
}

unsigned int Bounding_Volume_Hierarchy::M_calculate_exclusion_index(unsigned int _first_index, unsigned int _second_index) const
{
    L_ASSERT(_first_index != _second_index);

    if(_first_index > _second_index)
        std::swap(_first_index, _second_index);

    return (_first_index * m_registered_borders.total_size()) - M_calculate_exclusions_amount(_first_index + 1) + (_second_index - _first_index - 1);
}

void Bounding_Volume_Hierarchy::M_update_exclusions_cache() const
{
    unsigned int exclusions_size = M_calculate_exclusions_amount(m_registered_borders.total_size());
    if(exclusions_size == 0)
        return;

    if(m_exclusions.capacity() < exclusions_size)
        m_exclusions.resize(exclusions_size);

    m_exclusions.set_elements_amount(exclusions_size);
    m_exclusions.fill(false, exclusions_size);
}


Border Bounding_Volume_Hierarchy::M_construct_common_border(const IDs_Vec& _ids) const
{
    Border result;

    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        const Border& id_border = M_get_border(_ids[i]);
        result.expand_with(id_border);
    }

    return result;
}

Bounding_Volume_Hierarchy::IDs_Vec Bounding_Volume_Hierarchy::M_find_objects_inside(const Border& _border, const IDs_Vec& _ids) const
{
    IDs_Vec result(_ids.size());

    for(unsigned int i = 0; i < _ids.size(); ++i)
    {
        const Border& id_border = M_get_border(_ids[i]);

        if(_border.intersects_with(id_border))
            result.push(_ids[i]);
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


bool Bounding_Volume_Hierarchy::M_ids_share_group(unsigned int _id_0, unsigned int _id_1) const
{
    if(m_registered_borders.groups_amount() < 2)
        return false;

    unsigned int group_0 = m_registered_borders.calculate_group(_id_0);
    unsigned int group_1 = m_registered_borders.calculate_group(_id_1);

    return group_0 == group_1;
}


void Bounding_Volume_Hierarchy::M_append_pairs(const IDs_Vec& _ids) const
{
    for(unsigned int i_0 = 0; i_0 < _ids.size() - 1; ++i_0)
    {
        unsigned int id_0 = _ids[i_0];

        for(unsigned int i_1 = i_0 + 1; i_1 < _ids.size(); ++i_1)
        {
            unsigned int id_1 = _ids[i_1];

            unsigned int exclusion_index = M_calculate_exclusion_index(id_0, id_1);
            if(m_exclusions[exclusion_index])
                continue;
            m_exclusions[exclusion_index] = true;

            if(m_filter && !m_filter(id_0, id_1))
                continue;

            if(M_ids_share_group(id_0, id_1))
                continue;

            const Border& border_0 = M_get_border(id_0);
            const Border& border_1 = M_get_border(id_1);

            if(border_0.intersects_with(border_1))
                m_result.push({id_0, id_1});
        }
    }
}

void Bounding_Volume_Hierarchy::M_find_intersections(const Border& _border, const IDs_Vec& _ids_inside, unsigned int _level) const
{
    if(_ids_inside.size() < 2)
        return;

    if(_level >= m_max_depth || _ids_inside.size() <= m_min_objects_in_area)
        return M_append_pairs(_ids_inside);

    unsigned int max_component = M_find_largest_component(_border);
    LST::Border b_0 = M_split_border(_border, max_component, true);
    LST::Border b_1 = M_split_border(_border, max_component, false);

    IDs_Vec ids_0 = M_find_objects_inside(b_0, _ids_inside);
    IDs_Vec ids_1 = M_find_objects_inside(b_1, _ids_inside);

    if(M_compare_ids(ids_0, ids_1))
    {
        b_0 = M_construct_common_border(ids_0);

        M_find_intersections(b_0, ids_0, _level + 1);
    }
    else
    {
        b_0 = M_construct_common_border(ids_0);
        b_1 = M_construct_common_border(ids_1);

        M_find_intersections(b_0, ids_0, _level + 1);
        M_find_intersections(b_1, ids_1, _level + 1);
    }
}



const Bounding_Volume_Hierarchy::ID_Pairs_Vec& Bounding_Volume_Hierarchy::process() const
{
    m_result.mark_empty();

    if(m_registered_borders.total_size() < 2)
        return m_result;

    M_update_exclusions_cache();

    IDs_Vec initial_ids(m_registered_borders.total_size());
    for(unsigned int i = 0; i < m_registered_borders.total_size(); ++i)
        initial_ids.push(i);

    Border initial_border = M_construct_common_border(initial_ids);

    M_find_intersections(initial_border, initial_ids, 0);

    return m_result;
}
