#pragma once

#include <Data_Structures/Vector.h>
#include <Data_Structures/List.h>
#include <Data_Structures/AVL_Tree.h>


namespace LDS
{

    template<typename _Data_Type>
    Vector<_Data_Type> to_vector(const List<_Data_Type>& _list)
    {
        Vector<_Data_Type> result(_list.size());

        for(typename List<_Data_Type>::Const_Iterator it = _list.begin(); !it.end_reached(); ++it)
            result.push(*it);

        return result;
    }

    template<typename _Data_Type>
    Vector<_Data_Type> to_vector(const AVL_Tree<_Data_Type>& _tree)
    {
        Vector<_Data_Type> result(_tree.size());

        for(typename AVL_Tree<_Data_Type>::Const_Iterator it = _tree.iterator(); !it.end_reached(); ++it)
            result.push(*it);

        return result;
    }


    template<typename _Data_Type>
    List<_Data_Type> to_list(const Vector<_Data_Type>& _vector)
    {
        List<_Data_Type> result;

        for(unsigned int i = 0; i < _vector.size(); ++i)
            result.push_back(_vector[i]);

        return result;
    }

    template<typename _Data_Type>
    List<_Data_Type> to_list(const AVL_Tree<_Data_Type>& _tree)
    {
        List<_Data_Type> result;

        for(typename AVL_Tree<_Data_Type>::Const_Iterator it = _tree.iterator(); !it.end_reached(); ++it)
            result.push_back(*it);

        return result;
    }


    template<typename _Data_Type>
    AVL_Tree<_Data_Type> to_tree(const Vector<_Data_Type>& _vector)
    {
        AVL_Tree<_Data_Type> result;

        for(unsigned int i = 0; i < _vector.size(); ++i)
            result.insert(_vector[i]);

        return result;
    }

    template<typename _Data_Type>
    AVL_Tree<_Data_Type> to_tree(const List<_Data_Type>& _list)
    {
        AVL_Tree<_Data_Type> result(_list.size());

        for(typename List<_Data_Type>::Const_Iterator it = _list.begin(); !it.end_reached(); ++it)
            result.insert(*it);

        return result;
    }

}
