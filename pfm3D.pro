contains(QT_CONFIG, opengl): QT += opengl
QT += 
RESOURCES = icons.qrc
INCLUDEPATH += /c/PFM_ABEv7.0.0_Win64/include
LIBS += -L/c/PFM_ABEv7.0.0_Win64/lib -lnvutility -lgsf -lmisp -lpfm -lBinaryFeatureData -lgdal -lxml2 -lpoppler -lglu32 -lopengl32 -liconv -lwsock32
DEFINES += WIN32 NVWIN3X
CONFIG += console
CONFIG += static

#
# The following line is included so that the contents of acknowledgments.hpp will be available for translation
#

HEADERS += /c/PFM_ABEv7.0.0_Win64/include/acknowledgments.hpp

QMAKE_LFLAGS += 
######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 22 14:27:11 2020
######################################################################

TEMPLATE = app
TARGET = pfm3D
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += hotkeyHelp.hpp \
           pfm3D.hpp \
           pfm3DDef.hpp \
           pfm3DHelp.hpp \
           prefs.hpp \
           prefsHelp.hpp \
           version.hpp
SOURCES += adjust_bounds.cpp \
           checkFeature.cpp \
           env_in_out.cpp \
           geotiff.cpp \
           hotkeyHelp.cpp \
           main.cpp \
           overlayFlag.cpp \
           paint_otf_surface.cpp \
           paint_surface.cpp \
           pfm3D.cpp \
           prefs.cpp \
           set_defaults.cpp
RESOURCES += icons.qrc
TRANSLATIONS += pfm3D_xx.ts
