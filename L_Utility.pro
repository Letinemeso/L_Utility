TEMPLATE = lib
CONFIG += staticlib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "include"
INCLUDEPATH += "include/Data_Structures"

SOURCES += \
	source/Stuff/Stopwatch.cpp \
        source/Stuff/Timer.cpp \
        source/L_Debug/L_Debug.cpp \

HEADERS += \
	include/Data_Structures/AVL_Tree.h \
	include/Data_Structures/Binary_Heap.h \
	include/Data_Structures/List.h \
	include/Data_Structures/Map.h \
	include/Data_Structures/Tree.h \
	include/Data_Structures/Vector.h \
	include/Stuff/Function_Wrapper.h \
	include/Stuff/Stopwatch.h \
        include/Stuff/Timer.h \
        include/L_Debug/L_Debug.h \
