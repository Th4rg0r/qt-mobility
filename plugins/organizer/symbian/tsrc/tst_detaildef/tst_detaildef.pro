######################################################################
# Automatically generated by qmake (2.01a) Wed Jul 21 11:10:17 2010
######################################################################

TEMPLATE = app
TARGET = tst_detaildef
QT += testlib
CONFIG += qtestlib

symbian:
{
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE

    SOURCES += tst_detaildef.cpp

    CONFIG += mobility
    MOBILITY = organizer

    TARGET.CAPABILITY = ReadUserData \
                        WriteUserData

    LIBS += -lqtorganizer
}