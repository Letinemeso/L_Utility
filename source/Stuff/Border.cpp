#include <Stuff/Border.h>

using namespace LST;


void Border_Construction_Helper::reset()
{
    m_min = glm::vec3(std::numeric_limits<float>::max());
    m_max = glm::vec3(std::numeric_limits<float>::lowest());
}

void Border_Construction_Helper::consider_point_component(float _value, unsigned int _index)
{
    if(_value < m_min[_index])
        m_min[_index] = _value;
    if(_value > m_max[_index])
        m_max[_index] = _value;
}

void Border_Construction_Helper::consider_point(const glm::vec3& _point)
{
    for(unsigned int i = 0; i < 3; ++i)
        consider_point_component(_point[i], i);
}





Border::Border()
{
    invalidate();
}

Border::Border(const Border_Construction_Helper& _helper)
{
    init(_helper);
}

Border::Border(const LDS::Vector<float>& _raw_data)
{
    init(_raw_data);
}

Border::Border(const LDS::Vector<glm::vec3>& _points)
{
    init(_points);
}

Border::~Border()
{

}



void Border::init(const Border_Construction_Helper& _helper)
{
    set_values(_helper.calculate_center(), _helper.calculate_extents());
}

void Border::init(const LDS::Vector<float>& _raw_data)
{
    if(_raw_data.size() == 0)
        return;

    Border_Construction_Helper helper;

    for(unsigned int i = 0; i < _raw_data.size(); ++i)
        helper.consider_point_component(_raw_data[i], i % 3);

    init(helper);
}

void Border::init(const LDS::Vector<glm::vec3>& _points)
{
    if(_points.size() == 0)
        return;

    Border_Construction_Helper helper;

    for(unsigned int i = 0; i < _points.size(); ++i)
        helper.consider_point(_points[i]);

    init(helper);
}


void Border::set_values(const glm::vec3& _center, const glm::vec3& _extents)
{
    m_local_center = _center;
    m_local_extents = _extents;

    m_global_center = m_local_center;
    m_global_extents = m_local_extents;
}


void Border::update(const glm::mat4x4& _transformation)
{
    m_global_center = _transformation * glm::vec4(m_local_center, 1.0f);

    glm::mat3x3 transformation_without_movement = _transformation;
    m_global_extents = {0.0f, 0.0f, 0.0f};
    for(unsigned int i = 0; i < 3; ++i)
        m_global_extents += glm::abs(transformation_without_movement[i]) * m_local_extents[i];
}



bool Border::point_is_inside(const glm::vec3& _point) const
{
    if(!valid())
        return false;

    glm::vec3 offset_from_center = _point - m_global_center;

    for(unsigned int i = 0; i < 3; ++i)
    {
        if(fabsf(offset_from_center[i]) > m_global_extents[i])
            return false;
    }

    return true;
}

bool Border::intersects_with(const Border& _other) const
{
    if(!valid() || !_other.valid())
        return false;

    glm::vec3 vec_between_centers = m_global_center - _other.m_global_center;
    glm::vec3 extents_sum = m_global_extents + _other.m_global_extents;

    for(unsigned int i = 0; i < 3; ++i)
    {
        if(fabsf(vec_between_centers[i]) > extents_sum[i])
            return false;
    }

    return true;
}



Border& Border::expand_with(const Border& _other)
{
    if(!valid())
    {
        *this = _other;
        return *this;
    }

    glm::vec3 this_offset = m_global_center - m_global_extents;
    glm::vec3 this_limit = m_global_center + m_global_extents;

    glm::vec3 other_offset = _other.m_global_center - _other.m_global_extents;
    glm::vec3 other_limit = _other.m_global_center + _other.m_global_extents;

    glm::vec3 new_offset;
    glm::vec3 new_limit;

    for(unsigned int i = 0; i < 3; ++i)
    {
        new_offset[i] = std::min(this_offset[i], other_offset[i]);
        new_limit[i] = std::max(this_limit[i], other_limit[i]);
    }

    m_local_center = (new_limit + new_offset) * 0.5f;
    m_local_extents = (new_limit - new_offset) * 0.5f;
    m_global_center = m_local_center;
    m_global_extents = m_local_extents;

    return *this;
}


Border Border::operator&&(const Border& _other) const
{
    if(!valid() || !_other.valid())
        return {};

    if(!intersects_with(_other))
        return {};

    glm::vec3 resut_offset;
    glm::vec3 resut_limit;

    glm::vec3 this_offset = calculate_offset();
    glm::vec3 this_limit = calculate_limit();

    glm::vec3 other_offset = _other.calculate_offset();
    glm::vec3 other_limit = _other.calculate_limit();

    for(unsigned int i = 0; i < 3; ++i)
    {
        resut_offset[i] = std::max(this_offset[i], other_offset[i]);
        resut_limit[i] = std::min(this_limit[i], other_limit[i]);
    }

    Border_Construction_Helper helper;
    helper.consider_point(resut_offset);
    helper.consider_point(resut_limit);

    return Border(helper);
}

Border Border::operator||(const Border& _other) const
{
    if(!valid())
        return _other;
    if(!_other.valid())
        return {};

    Border result = *this;
    result.expand_with(_other);
    return result;
}
