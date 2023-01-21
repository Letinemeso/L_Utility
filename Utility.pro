TEMPLATE = lib
CONFIG += staticlib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "include"
INCLUDEPATH += "include/Data_Structures"
INCLUDEPATH += "include/Debug"

SOURCES += \
	source/Data_Structures/AVL_Tree.cpp \
	source/Data_Structures/Binary_Heap.cpp \
	source/Data_Structures/List.cpp \
	source/Data_Structures/Map.cpp \
	source/Data_Structures/Tree.cpp \
	source/Data_Structures/Vector.cpp \
	source/Stuff/Stopwatch.cpp \
	source/L_Debug/L_Debug.cpp \

HEADERS += \
	include/Data_Structures/AVL_Tree.h \
	include/Data_Structures/Binary_Heap.h \
	include/Data_Structures/List.h \
	include/Data_Structures/Map.h \
	include/Data_Structures/Tree.h \
	include/Data_Structures/Vector.h \
	include/Stuff/Stopwatch.h \
	include/L_Debug/L_Debug.h \
