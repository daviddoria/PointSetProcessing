CONFIG      += designer plugin

TARGET      = $$qtLibraryTarget($$TARGET)

TEMPLATE    = lib

QTDIR_build:DESTDIR     = $$QT_BUILD_TREE/plugins/designer


HEADERS     = FloatSlider.h \
              FloatSliderPlugin.h \
              LabeledSlider.h
SOURCES     = FloatSlider.cpp \
              LabeledSlider.cpp \
              FloatSliderPlugin.cpp
FORMS       = LabeledSlider.ui # The widget layout for FloatSlider is exactly the same as LabeledSlider


# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/FloatSliderPlugin
INSTALLS += target sources

symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
