QT       += core gui xml sql network

#CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classifier.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    classifier.h \
    mainwindow.h
HEADERS += lib/kvs/kvstore.h \
           lib/log/clog.h \
           lib/report/reportcreator.h \
           lib/xml/xpath.h \
           lib/db/src/dbmanager.h \
           lib/gui/src/extcombobox.h \
           lib/gui/src/extgview.h \
           lib/gui/src/extimageviewer.h \
           lib/gui/src/imagegroupview.h \
           lib/gui/src/imagelistmodel.h \
           lib/gui/src/imageview.h \
           lib/gui/src/markeritem.h \
           lib/gui/src/valuecontrols.h \
           lib/imgdb/src/imgdb.h \
           lib/os/src/filewatcher.h \
           lib/os/src/fstools.h \
           lib/os/src/persistence.h \
           lib/os/src/siserver.h
FORMS += lib/gui/src/imagegroupview.ui
SOURCES += lib/kvs/kvstore.cpp \
           lib/log/clog.cpp \
           lib/report/reportcreator.cpp \
           lib/xml/xpath.cpp \
           lib/db/src/dbmanager.cpp \
           lib/gui/src/extcombobox.cpp \
           lib/gui/src/extgview.cpp \
           lib/gui/src/extimageviewer.cpp \
           lib/gui/src/imagegroupview.cpp \
           lib/gui/src/imagelistmodel.cpp \
           lib/gui/src/imageview.cpp \
           lib/gui/src/markeritem.cpp \
           lib/gui/src/objectselector.cpp \
           lib/gui/src/valuecontrols.cpp \
           lib/imgdb/src/imgdb.cpp \
           lib/os/src/filewatcher.cpp \
           lib/os/src/fstools.cpp \
           lib/os/src/persistence.cpp \
           lib/os/src/siserver.cpp

			   

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    SimpleClassifier_ru_RU.ts




INCLUDEPATH += . "C:/LIB/opencv24/include" ./lib/kvs/ ./lib/os/src/ ./lib/gui/src/ ./lib/imgdb/



win32 {
	CONFIG(debug, debug|release) {		
		LIBS += \
		C:/LIB/opencv24/lib/opencv_contrib240d.lib \
		C:/LIB/opencv24/lib/opencv_core240d.lib \
		C:/LIB/opencv24/lib/opencv_features2d240d.lib \
		C:/LIB/opencv24/lib/opencv_highgui240d.lib \
		C:/LIB/opencv24/lib/opencv_imgproc240d.lib \
		C:/LIB/opencv24/lib/opencv_ml240d.lib \
		C:/LIB/opencv24/lib/opencv_objdetect240d.lib 
        }
}	
