QT       += core network gui
TEMPLATE = app
TARGET = spe4k

INCLUDEPATH += . \
               ../lib/libspe4rc/src

# Input
HEADERS += common/common.hh \
           common/command.hh \
           common/command.hxx \
           common/command-handler.hh \
           common/default-parameters.hh \
           common/parameters.hh \
           common/parameters.hxx \
           common/text-display.hh \
           common/text-display.hxx \
           common/text-formatter.hh \
           widget/dialog/color-label.hh \
           widget/dialog/event-display.hh \
           widget/dialog/network-dialog.hh \
           widget/dialog/network-tree-item.hh \
           widget/dialog/option-dialog.hh \
           widget/dialog/sessionlist-dialog.hh \
           widget/main-window.hh \
           widget/session-area.hh \
           widget/window/channel-window.hh \
           widget/window/edit-box.hh \
           widget/window/status-subwindow.hh \
           widget/window/status-window.hh \
           widget/window/text-box.hh \
           widget/window/user-item.hh \
           widget/window/user-window.hh \
           widget/window/window.hh \
           widget/window/window-item.hh \


SOURCES += spe4k.cc \
           common/parameters.cc \
           common/command-handler.cc \
           common/text-display.cc \
           common/text-formatter.cc \
           widget/dialog/color-label.cc \
           widget/dialog/event-display.cc \
           widget/dialog/network-dialog.cc \
           widget/dialog/network-tree-item.cc \
           widget/dialog/option-dialog.cc \
           widget/dialog/sessionlist-dialog.cc \
           widget/main-window.cc \
           widget/session-area.cc \
           widget/window/channel-window.cc \
           widget/window/edit-box.cc \
           widget/window/status-window.cc \
           widget/window/text-box.cc \
           widget/window/user-item.cc \
           widget/window/user-window.cc \
           widget/window/window.cc \
           widget/window/window-item.cc


FORMS += ui/channel-window.ui \
         ui/edit-box.ui \
         ui/event-display.ui \
         ui/main-window.ui \
         ui/network-dialog.ui \
         ui/option-dialog.ui \
         ui/session-area.ui \
         ui/sessionlist-dialog.ui \
         ui/status-window.ui \
         ui/text-box.ui \
         ui/user-window.ui \
		 ui/color-label.ui


CONFIG(debug, debug|release) {
        debug:DESTDIR = ..
        debug:OBJECTS_DIR = ../build/debug/.obj
        debug:MOC_DIR = ../build/debug/.moc
        debug:RCC_DIR = ../build/debug/.rcc
        debug:UI_DIR = ../build/debug/.ui
}

CONFIG(release, debug|release) {
        release:DESTDIR = ..
        release:OBJECTS_DIR = ../build/release/.obj
        release:MOC_DIR = ../build/release/.moc
        release:RCC_DIR = ../build/release/.rcc
        release:UI_DIR = ../build/release/.ui
}

DEPENDENCY_LIBRARIES = spe4rc


RESOURCES += \
    ../icons.qrc

include(./deps.pri)
