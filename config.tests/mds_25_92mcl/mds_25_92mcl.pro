CONFIG -= qt
TEMPLATE = app

# Input
SOURCES += main.cpp

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
LIBS +=     -lmdeclient.dll
