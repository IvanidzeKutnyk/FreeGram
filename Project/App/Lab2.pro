QT        += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    add_info.cpp \
    main.cpp \
    mainwindow.cpp \
    search_customer.cpp \
    total_main.cpp

HEADERS += \
    add_info.h \
    mainwindow.h \
    search_customer.h \
    total_main.h

FORMS += \
    add_info.ui \
    mainwindow.ui \
    search_customer.ui \
    total_main.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
