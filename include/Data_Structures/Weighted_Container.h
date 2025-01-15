#pragma once

#include <Data_Structures/Map.h>


namespace LDS
{

    template<typename Data_Type>
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

        using Weights_Map = LDS::Map<Weights_Range, Data_Type>;

    private:
        Weights_Map m_weights_map;
        unsigned int m_total_weight = 0;

    public:
        Weighted_Container();
        ~Weighted_Container();

    public:
        void add_element(const Data_Type& _data, unsigned int _weight);
        void add_element(Data_Type&& _data, unsigned int _weight);
        void clear_elements();

    public:
        inline unsigned int total_weight() const { return m_total_weight; }
        inline const Weights_Map& weights_map() const { return m_weights_map; }

    public:
        const Data_Type& get_by_weight(unsigned int _weight) const;

    };



    //

    template<typename Data_Type>
    Weighted_Container<Data_Type>::Weighted_Container()
    {

    }

    template<typename Data_Type>
    Weighted_Container<Data_Type>::~Weighted_Container()
    {

    }



    template<typename Data_Type>
    void Weighted_Container<Data_Type>::add_element(const Data_Type& _data, unsigned int _weight)
    {
        m_weights_map.insert( { m_total_weight, m_total_weight + _weight - 1 }, _data );
        m_total_weight += _weight;
    }

    template<typename Data_Type>
    void Weighted_Container<Data_Type>::add_element(Data_Type&& _data, unsigned int _weight)
    {
        m_weights_map.insert( { m_total_weight, m_total_weight + _weight - 1 }, (Data_Type&&)_data );
        m_total_weight += _weight;
    }

    template<typename Data_Type>
    void Weighted_Container<Data_Type>::clear_elements()
    {
        m_weights_map.clear();
        m_total_weight = 0;
    }



    template<typename Data_Type>
    const Data_Type& Weighted_Container<Data_Type>::get_by_weight(unsigned int _weight) const
    {
        typename Weights_Map::Const_Iterator data = m_weights_map.find(_weight);
        L_ASSERT(data.is_ok());

        return *data;
    }

}
