#ifndef LIST_H
#define LIST_H

#include <L_Debug/L_Debug.h>


namespace LDS
{

	template<typename Data_Type>
	class List
	{
	private:
		struct Node
		{
			Data_Type data;

			Node* next = nullptr;
			Node* prev = nullptr;

			Node(Data_Type&& _data) : data((Data_Type&&)_data) { }
			Node(const Data_Type& _data) : data(_data) { }
		};

	private:
		class Iterator_Base final
		{
		private:
			friend class LDS::List<Data_Type>;

		private:
			const LDS::List<Data_Type>* m_parent = nullptr;

			LDS::List<Data_Type>::Node* m_current_pos = nullptr;

			bool m_begin_reached = false;
			bool m_end_reached = false;

		public:
			Iterator_Base(){}
			Iterator_Base(const LDS::List<Data_Type>* _parent);
			Iterator_Base(const Iterator_Base& _other);
			void operator=(const Iterator_Base& _other);
			~Iterator_Base();

		public:
			void operator++();
			void operator--();

		public:
			inline Data_Type& operator*();
			inline const Data_Type& operator*() const;
			inline Data_Type* get_ptr();
			inline const Data_Type* get_ptr() const;

		public:
			inline bool begin_reached() const;
			inline bool end_reached() const;

			inline bool is_ok() const;

		};

	public:
		class Iterator final
		{
		private:
			friend class LDS::List<Data_Type>;

		private:
			Iterator_Base m_it;

		private:
			Iterator(LDS::List<Data_Type>* _parent);

		public:
            Iterator(){}
			Iterator(const Iterator& _other);
			void operator=(const Iterator& _other);

		public:
			void operator++();
			void operator--();

		public:
			Data_Type& operator*();
			const Data_Type& operator*() const;
			Data_Type* operator->();
			const Data_Type* operator->() const;

		public:
			bool begin_reached() const;
			bool end_reached() const;

			bool is_ok() const;

		};

		class Const_Iterator final
		{
		private:
			friend class LDS::List<Data_Type>;

		private:
			Iterator_Base m_it;

		private:
			Const_Iterator(const LDS::List<Data_Type>* _parent);

		public:
			Const_Iterator(const Const_Iterator& _other);
			void operator=(const Const_Iterator& _other);

		public:
			void operator++();
			void operator--();

		public:
            Data_Type& operator*();
			const Data_Type& operator*() const;
            Data_Type* operator->();
            const Data_Type* operator->() const;

		public:
			bool begin_reached() const;
			bool end_reached() const;

			bool is_ok() const;

		};

	private:
		Node* m_head = nullptr;
		Node* m_tail = nullptr;

		unsigned int m_size = 0;

	public:
		List();
		List(const List& _other);
		List(List&& _other);

		void operator=(const List& _other);
		void operator=(List&& _other);

		virtual ~List();

	public:
		void push_back(const Data_Type& _data);
		void push_back(Data_Type&& _data);
		void push_front(const Data_Type& _data);
		void push_front(Data_Type&& _data);

        void push_after(const Iterator& _pos, const Data_Type& _data);
        void push_after(const Iterator& _pos, Data_Type&& _data);
        void push_before(const Iterator& _pos, const Data_Type& _data);
        void push_before(const Iterator& _pos, Data_Type&& _data);

		void pop_back();
		void pop_front();

		void erase(const Iterator& _where);
        Iterator erase_and_iterate_forward(const Iterator& _where);

		void clear();

		void append(const List<Data_Type>& _other);
		void append(List<Data_Type>&& _other);

	public:
		inline unsigned int size() const { return m_size; }

		Iterator begin();
		Const_Iterator begin() const;
		Iterator end();
		Const_Iterator end() const;

        Iterator find(const Data_Type& _what);
        Const_Iterator find(const Data_Type& _what) const;

	};


    //	List::Iterator_Base

    template<typename Data_Type>
    List<Data_Type>::Iterator_Base::Iterator_Base(const List<Data_Type>* _parent)
        : m_parent(_parent)
    {

    }

    template<typename Data_Type>
    List<Data_Type>::Iterator_Base::Iterator_Base(const Iterator_Base& _other)
        : m_parent(_other.m_parent), m_current_pos(_other.m_current_pos), m_begin_reached(_other.m_begin_reached), m_end_reached(_other.m_end_reached)
    {

    }

    template<typename Data_Type>
    void List<Data_Type>::Iterator_Base::operator=(const Iterator_Base& _other)
    {
        m_parent = _other.m_parent;
        m_current_pos = _other.m_current_pos;
        m_begin_reached = _other.m_begin_reached;
        m_end_reached = _other.m_end_reached;
    }

    template<typename Data_Type>
    List<Data_Type>::Iterator_Base::~Iterator_Base()
    {

    }



    template<typename Data_Type>
    void List<Data_Type>::Iterator_Base::operator++()
    {
        L_ASSERT(!m_end_reached);

        m_begin_reached = false;

        if(m_current_pos->next == nullptr)
        {
            m_end_reached = true;
            return;
        }

        m_current_pos = m_current_pos->next;
    }

    template<typename Data_Type>
    void List<Data_Type>::Iterator_Base::operator--()
    {
        L_ASSERT(!m_begin_reached);

        m_end_reached = false;

        if(m_current_pos->prev == nullptr)
        {
            m_begin_reached = true;
            return;
        }

        m_current_pos = m_current_pos->prev;
    }



    template<typename Data_Type>
    Data_Type& List<Data_Type>::Iterator_Base::operator*()
    {
        L_ASSERT(is_ok());

        return m_current_pos->data;
    }

    template<typename Data_Type>
    const Data_Type& List<Data_Type>::Iterator_Base::operator*() const
    {
        L_ASSERT(is_ok());

        return m_current_pos->data;
    }

    template<typename Data_Type>
    Data_Type* List<Data_Type>::Iterator_Base::get_ptr()
    {
        L_ASSERT(is_ok());

        return &m_current_pos->data;
    }

    template<typename Data_Type>
    const Data_Type* List<Data_Type>::Iterator_Base::get_ptr() const
    {
        L_ASSERT(is_ok());

        return &m_current_pos->data;
    }



    template<typename Data_Type>
    bool List<Data_Type>::Iterator_Base::begin_reached() const
    {
        if(!is_ok())
            return true;
        return m_begin_reached;
    }

    template<typename Data_Type>
    bool List<Data_Type>::Iterator_Base::end_reached() const
    {
        if(!is_ok())
            return true;
        return m_end_reached;
    }


    template<typename Data_Type>
    bool List<Data_Type>::Iterator_Base::is_ok() const
    {
        return (m_current_pos) && (m_parent);
    }





    //	List::Iterator

    template<typename Data_Type>
    List<Data_Type>::Iterator::Iterator(List<Data_Type>* _parent)
        : m_it(_parent)
    {

    }



    template<typename Data_Type>
    List<Data_Type>::Iterator::Iterator(const Iterator& _other)
        : m_it(_other.m_it)
    {

    }

    template<typename Data_Type>
    void List<Data_Type>::Iterator::operator=(const Iterator& _other)
    {
        m_it = _other.m_it;
    }



    template<typename Data_Type>
    void List<Data_Type>::Iterator::operator++()
    {
        ++m_it;
    }

    template<typename Data_Type>
    void List<Data_Type>::Iterator::operator--()
    {
        --m_it;
    }



    template<typename Data_Type>
    Data_Type& List<Data_Type>::Iterator::operator*()
    {
        return *m_it;
    }

    template<typename Data_Type>
    const Data_Type& List<Data_Type>::Iterator::operator*() const
    {
        return *m_it;
    }

    template<typename Data_Type>
    Data_Type* List<Data_Type>::Iterator::operator->()
    {
        return m_it.get_ptr();
    }

    template<typename Data_Type>
    const Data_Type* List<Data_Type>::Iterator::operator->() const
    {
        return m_it.get_ptr();
    }



    template<typename Data_Type>
    bool List<Data_Type>::Iterator::begin_reached() const
    {
        return m_it.begin_reached();
    }

    template<typename Data_Type>
    bool List<Data_Type>::Iterator::end_reached() const
    {
        return m_it.end_reached();
    }


    template<typename Data_Type>
    bool List<Data_Type>::Iterator::is_ok() const
    {
        return m_it.is_ok();
    }





    //	List::Const_Iterator

    template<typename Data_Type>
    List<Data_Type>::Const_Iterator::Const_Iterator(const List<Data_Type>* _parent)
        : m_it(_parent)
    {

    }



    template<typename Data_Type>
    List<Data_Type>::Const_Iterator::Const_Iterator(const Const_Iterator& _other)
        : m_it(_other.m_it)
    {

    }

    template<typename Data_Type>
    void List<Data_Type>::Const_Iterator::operator=(const Const_Iterator& _other)
    {
        m_it = _other.m_it;
    }



    template<typename Data_Type>
    void List<Data_Type>::Const_Iterator::operator++()
    {
        ++m_it;
    }

    template<typename Data_Type>
    void List<Data_Type>::Const_Iterator::operator--()
    {
        --m_it;
    }



    template<typename Data_Type>
    Data_Type& List<Data_Type>::Const_Iterator::operator*()
    {
        return *m_it;
    }

    template<typename Data_Type>
    const Data_Type& List<Data_Type>::Const_Iterator::operator*() const
    {
        return *m_it;
    }

    template<typename Data_Type>
    Data_Type* List<Data_Type>::Const_Iterator::operator->()
    {
        return m_it.get_ptr();
    }

    template<typename Data_Type>
    const Data_Type* List<Data_Type>::Const_Iterator::operator->() const
    {
        return m_it.get_ptr();
    }



    template<typename Data_Type>
    bool List<Data_Type>::Const_Iterator::begin_reached() const
    {
        return m_it.begin_reached();
    }

    template<typename Data_Type>
    bool List<Data_Type>::Const_Iterator::end_reached() const
    {
        return m_it.end_reached();
    }


    template<typename Data_Type>
    bool List<Data_Type>::Const_Iterator::is_ok() const
    {
        return m_it.is_ok();
    }



    //	List

    template<typename Data_Type>
    List<Data_Type>::List()
    {

    }

    template<typename Data_Type>
    List<Data_Type>::List(const List& _other)
    {
        const Node* other_tptr = _other.m_head;
        while(other_tptr != nullptr)
        {
            push_back(other_tptr->data);
            other_tptr = other_tptr->next;
        }
    }

    template<typename Data_Type>
    List<Data_Type>::List(List&& _other)
    {
        m_size = _other.m_size;
        _other.m_size = 0;
        m_head = _other.m_head;
        _other.m_head = nullptr;
        m_tail = _other.m_tail;
        _other.m_tail = nullptr;
    }


    template<typename Data_Type>
    void List<Data_Type>::operator=(const List& _other)
    {
        m_size = _other.m_size;
        const Node* other_tptr = _other.m_head;
        while(other_tptr != nullptr)
        {
            push_back(other_tptr->data);
            other_tptr = other_tptr->next;
        }
    }

    template<typename Data_Type>
    void List<Data_Type>::operator=(List&& _other)
    {
        m_size = _other.m_size;
        _other.m_size = 0;
        m_head = _other.m_head;
        _other.m_head = nullptr;
        m_tail = _other.m_tail;
        _other.m_tail = nullptr;
    }


    template<typename Data_Type>
    List<Data_Type>::~List()
    {
        clear();
    }



    template<typename Data_Type>
    void List<Data_Type>::push_back(const Data_Type& _data)
    {
        Node* node = new Node(_data);

        if(m_tail == nullptr)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->next = node;
            node->prev = m_tail;
            m_tail = m_tail->next;
        }

        ++m_size;
    }

    template<typename Data_Type>
    void List<Data_Type>::push_back(Data_Type&& _data)
    {
        Node* node = new Node((Data_Type&&)_data);

        if(m_tail == nullptr)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->next = node;
            node->prev = m_tail;
            m_tail = node;
        }

        ++m_size;
    }

    template<typename Data_Type>
    void List<Data_Type>::push_front(const Data_Type& _data)
    {
        Node* node = new Node(_data);

        if(m_head == nullptr)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_head->prev = node;
            node->next = m_head;
            m_head = node;
        }

        ++m_size;
    }

    template<typename Data_Type>
    void List<Data_Type>::push_front(Data_Type&& _data)
    {
        Node* node = new Node((Data_Type&&)_data);

        if(m_tail == nullptr)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_head->prev = node;
            node->next = m_head;
            m_head = node;
        }

        ++m_size;
    }


    template<typename Data_Type>
    void List<Data_Type>::push_after(const Iterator& _pos, const Data_Type& _data)
    {
        L_ASSERT(_pos.m_it.m_parent == this);

        if(_pos.begin_reached())
        {
            push_front(_data);
        }
        else if(_pos.end_reached())
        {
            push_back(_data);
        }
        else
        {
            Node* pos = _pos.m_it.m_current_pos;
            Node* prev_next = pos->next;
            Node* new_next = new Node(_data);

            pos->next = new_next;
            new_next->next = prev_next;
            new_next->prev = pos;

            if(prev_next)
                prev_next->prev = new_next;

            ++m_size;
        }

    }

    template<typename Data_Type>
    void List<Data_Type>::push_after(const Iterator& _pos, Data_Type&& _data)
    {
        L_ASSERT(_pos.m_it.m_parent == this);

        if(_pos.begin_reached())
        {
            push_front((Data_Type&&)_data);
        }
        else if(_pos.end_reached())
        {
            push_back((Data_Type&&)_data);
        }
        else
        {
            Node* pos = _pos.m_it.m_current_pos;
            Node* prev_next = pos->next;
            Node* new_next = new Node((Data_Type&&)_data);

            pos->next = new_next;
            new_next->next = prev_next;
            new_next->prev = pos;

            if(prev_next)
                prev_next->prev = new_next;

            ++m_size;
        }

    }

    template<typename Data_Type>
    void List<Data_Type>::push_before(const Iterator& _pos, const Data_Type& _data)
    {
        L_ASSERT(_pos.m_it.m_parent == this);

        if(_pos.begin_reached())
        {
            push_front(_data);
        }
        else if(_pos.end_reached())
        {
            push_back(_data);
        }
        else
        {
            Node* pos = _pos.m_it.m_current_pos;
            Node* prev_prev = pos->prev;
            Node* new_prev = new Node(_data);

            pos->prev = new_prev;
            new_prev->next = pos;
            new_prev->prev = prev_prev;

            if(prev_prev)
                prev_prev->next = new_prev;

            ++m_size;
        }
    }

    template<typename Data_Type>
    void List<Data_Type>::push_before(const Iterator& _pos, Data_Type&& _data)
    {
        L_ASSERT(_pos.m_it.m_parent == this);

        if(_pos.begin_reached())
        {
            push_front((Data_Type&&)_data);
        }
        else if(_pos.end_reached())
        {
            push_back((Data_Type&&)_data);
        }
        else
        {
            Node* pos = _pos.m_it.m_current_pos;
            Node* prev_prev = pos->prev;
            Node* new_prev = new Node((Data_Type&&)_data);

            pos->prev = new_prev;
            new_prev->next = pos;
            new_prev->prev = prev_prev;

            if(prev_prev)
                prev_prev->next = new_prev;

            ++m_size;
        }
    }


    template<typename Data_Type>
    void List<Data_Type>::pop_back()
    {
        L_ASSERT(m_head);

        if(m_size == 1)
        {
            delete m_head;
            m_head = nullptr;
            m_tail = nullptr;
        }
        else
        {
            Node* node = m_tail;
            m_tail = m_tail->prev;
            m_tail->next = nullptr;
            delete node;
        }

        --m_size;
    }

    template<typename Data_Type>
    void List<Data_Type>::pop_front()
    {
        L_ASSERT(m_head);

        if(m_size == 1)
        {
            delete m_head;
            m_head = nullptr;
            m_tail = nullptr;
        }
        else
        {
            Node* node = m_head;
            m_head = m_head->next;
            m_head->prev = nullptr;
            delete node;
        }

        --m_size;
    }


    template<typename Data_Type>
    void List<Data_Type>::erase(const Iterator& _where)
    {
        L_ASSERT(m_head);
        L_ASSERT(_where.is_ok());
        L_ASSERT(_where.m_it.m_parent == this);

        if(m_size == 1)
        {
            L_ASSERT(_where.m_it.m_current_pos == m_head);
            delete m_head;
            m_head = nullptr;
            m_tail = nullptr;
        }
        else if(_where.m_it.m_current_pos == m_head)
        {
            Node* node = m_head;
            m_head = m_head->next;
            m_head->prev = nullptr;
            delete node;
        }
        else if(_where.m_it.m_current_pos == m_tail)
        {
            Node* node = m_tail;
            m_tail = m_tail->prev;
            m_tail->next = nullptr;
            delete node;
        }
        else
        {
            Node* cur = _where.m_it.m_current_pos;
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            delete cur;
        }

        --m_size;
    }

    template<typename Data_Type>
    typename List<Data_Type>::Iterator List<Data_Type>::erase_and_iterate_forward(const Iterator& _where)
    {
        L_ASSERT(m_head);
        L_ASSERT(_where.is_ok());
        L_ASSERT(_where.m_it.m_parent == this);

        Iterator result(this);

        if(m_size == 1)
        {
            L_ASSERT(_where.m_it.m_current_pos == m_head);
            delete m_head;
            m_head = nullptr;
            m_tail = nullptr;
        }
        else if(_where.m_it.m_current_pos == m_head)
        {
            Node* node = m_head;
            m_head = m_head->next;
            m_head->prev = nullptr;
            result.m_it.m_current_pos = m_head;
            result.m_it.m_begin_reached = true;
            delete node;
        }
        else if(_where.m_it.m_current_pos == m_tail)
        {
            Node* node = m_tail;
            m_tail = m_tail->prev;
            m_tail->next = nullptr;
            result.m_it.m_current_pos = m_tail;
            result.m_it.m_end_reached = true;
            delete node;
        }
        else
        {
            Node* cur = _where.m_it.m_current_pos;
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            result.m_it.m_current_pos = cur->next;
            result.m_it.m_begin_reached = false;
            result.m_it.m_end_reached = false;
            delete cur;
        }

        --m_size;

        return result;
    }


    template<typename Data_Type>
    void List<Data_Type>::clear()
    {
        while(m_head != nullptr)
        {
            Node* tptr = m_head;
            m_head = m_head->next;
            delete tptr;
        }
        m_tail = nullptr;
        m_size = 0;
    }


    template<typename Data_Type>
    void List<Data_Type>::append(const List<Data_Type>& _other)
    {
        Node* ptr = _other.m_head;
        while(ptr != nullptr)
        {
            push_back(ptr->data);
            ptr = ptr->next;
        }
    }

    template<typename Data_Type>
    void List<Data_Type>::append(List<Data_Type>&& _other)
    {
        m_size += _other.m_size;

        if(m_head == nullptr)
        {
            m_head = _other.m_head;
            m_tail = _other.m_tail;
        }
        else
        {
            m_tail->next = _other.m_head;
            if(_other.m_tail != nullptr)
                m_tail = _other.m_tail;
        }

        _other.m_head = nullptr;
        _other.m_tail = nullptr;
        _other.m_size = 0;
    }



    template<typename Data_Type>
    typename List<Data_Type>::Iterator List<Data_Type>::begin()
    {
        Iterator result(this);
        result.m_it.m_current_pos = m_head;
        return result;
    }

    template<typename Data_Type>
    typename List<Data_Type>::Const_Iterator List<Data_Type>::begin() const
    {
        Const_Iterator result(this);
        result.m_it.m_current_pos = m_head;
        return result;
    }

    template<typename Data_Type>
    typename List<Data_Type>::Iterator List<Data_Type>::end()
    {
        Iterator result(this);
        result.m_it.m_current_pos = m_tail;
        result.m_it.m_begin_reached = false;
        result.m_it.m_end_reached = true;
        return result;
    }

    template<typename Data_Type>
    typename List<Data_Type>::Const_Iterator List<Data_Type>::end() const
    {
        Const_Iterator result(this);
        result.m_it.m_current_pos = m_tail;
        result.m_it.m_begin_reached = false;
        result.m_it.m_end_reached = true;
        return result;
    }


    template<typename Data_Type>
    typename List<Data_Type>::Iterator List<Data_Type>::find(const Data_Type& _what)
    {
        Iterator result = begin();
        while(!result.end_reached())
        {
            if(*result == _what)
                break;
            ++result;
        }
        return result;
    }

    template<typename Data_Type>
    typename List<Data_Type>::Const_Iterator List<Data_Type>::find(const Data_Type& _what) const
    {
        Const_Iterator result = begin();
        while(!result.end_reached())
        {
            if(*result == _what)
                break;
            ++result;
        }
        return result;
    }

}



#endif // LIST_H
