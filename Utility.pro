TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Data_Structures/AVL_Tree.cpp \
        Data_Structures/Binary_Heap.cpp \
        Data_Structures/Tree.cpp \
        Data_Structures/Vector.cpp \
        Debug/Debug.cpp \
        main.cpp

HEADERS += \
	Data_Structures/AVL_Tree.h \
	Data_Structures/Binary_Heap.h \
	Data_Structures/Tree.h \
	Data_Structures/Vector.h \
	Debug/Debug.h
