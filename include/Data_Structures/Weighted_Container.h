#pragma once

#include <Data_Structures/Vector.h>
#include <Data_Structures/Map.h>


namespace LDS
{

    template<typename _Data_Type>
    class Weighted_Container
    {
    public:
        struct Weights_Range
        {
            unsigned int min = 0;
            unsigned int max = 0;

            inline bool operator<(const Weights_Range& _other) const { return max < _other.min; }
            inline bool operator>(const Weights_Range& _other) const { return min > _other.max; }
            inline bool operator==(const Weights_Range& _other) const { return (min >= _other.min && min <= _other.max) || (max >= _other.min && max <= _other.max); }

            inline bool operator<(unsigned int _weight) const { return max < _weight; }
            inline bool operator>(unsigned int _weight) const { return min > _weight; }
            inline bool operator==(unsigned int _weight) const { return _weight >= min && _weight <= max; }
        };

        using Weights_Map = LDS::Map<Weights_Range, _Data_Type>;

    private:
        Weights_Map m_weights_map;
        unsigned int m_total_weight = 0;

    public:
        Weighted_Container();
        ~Weighted_Container();

    public:
        void fill_from(const LDS::Vector<_Data_Type>& _data, const LDS::Vector<unsigned int>& _weights);
        void add_element(const _Data_Type& _data, unsigned int _weight);
        void add_element(_Data_Type&& _data, unsigned int _weight);
        void clear_elements();

    public:
        inline unsigned int max_weight() const { return m_total_weight - 1; }
        inline unsigned int total_weight() const { return m_total_weight; }
        inline const Weights_Map& weights_map() const { return m_weights_map; }
        inline bool empty() const { return m_total_weight == 0; }

    public:
        const _Data_Type& get_by_weight(unsigned int _weight) const;
        const _Data_Type& get_random() const;

    };



    //

    template<typename _Data_Type>
    Weighted_Container<_Data_Type>::Weighted_Container()
    {

    }

    template<typename _Data_Type>
    Weighted_Container<_Data_Type>::~Weighted_Container()
    {

    }



    template<typename _Data_Type>
    void Weighted_Container<_Data_Type>::fill_from(const LDS::Vector<_Data_Type>& _data, const LDS::Vector<unsigned int>& _weights)
    {
        clear_elements();

        L_ASSERT(_data.size() == _weights.size());

        for(unsigned int i = 0; i < _data.size(); ++i)
            add_element( _data[i], _weights[i] );
    }

    template<typename _Data_Type>
    void Weighted_Container<_Data_Type>::add_element(const _Data_Type& _data, unsigned int _weight)
    {
        m_weights_map.insert( { m_total_weight, m_total_weight + _weight - 1 }, _data );
        m_total_weight += _weight;
    }

    template<typename _Data_Type>
    void Weighted_Container<_Data_Type>::add_element(_Data_Type&& _data, unsigned int _weight)
    {
        m_weights_map.insert( { m_total_weight, m_total_weight + _weight - 1 }, (_Data_Type&&)_data );
        m_total_weight += _weight;
    }

    template<typename _Data_Type>
    void Weighted_Container<_Data_Type>::clear_elements()
    {
        m_weights_map.clear();
        m_total_weight = 0;
    }



    template<typename _Data_Type>
    const _Data_Type& Weighted_Container<_Data_Type>::get_by_weight(unsigned int _weight) const
    {
        LST::Function<bool(const unsigned int&, const typename Weights_Map::Pair&)> less_func = [](const unsigned int& _weight, const typename Weights_Map::Pair& _pair)
        {
            return _pair.key() > _weight;
        };
        LST::Function<bool(const unsigned int&, const typename Weights_Map::Pair&)> equals_func = [](const unsigned int& _weight, const typename Weights_Map::Pair& _pair)
        {
            return _pair.key() == _weight;
        };

        typename Weights_Map::Const_Iterator data = m_weights_map.find(less_func, equals_func, _weight);
        L_ASSERT(data.is_ok());

        return *data;
    }

    template<typename _Data_Type>
    const _Data_Type& Weighted_Container<_Data_Type>::get_random() const
    {
        unsigned int random_weight = 0;
        if(m_total_weight > 1)
            random_weight = rand() % m_total_weight;
        return get_by_weight(random_weight);
    }

}
