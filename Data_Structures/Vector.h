#ifndef VECTOR_H
#define VECTOR_H


namespace LDS
{

	template<typename Data_Type>
	class Vector final
	{
	private:
		class Basic_Iterator final
		{
		private:
			Vector<Data_Type>* m_parent = nullptr;

		public:
			Basic_Iterator(Vector<Data_Type>* _parent);
			Basic_Iterator(const Basic_Iterator& _other);
			void operator=(const Basic_Iterator& _other);

		public:
			void operator++();
			void operator--();

		public:
			Data_Type& operator*();
			const Data_Type& operator*() const;

		};

	public:
		class Iterator final
		{

		};

	private:
		Data_Type** m_array = nullptr;
		unsigned int m_elements_count = 0;
		unsigned int m_size = 0;

	public:
		Vector();
		~Vector();

	public:
		void push(const Data_Type& _data);
		void push(Data_Type&& _data);

		void pop();

	public:

	};

}


#endif // VECTOR_H
