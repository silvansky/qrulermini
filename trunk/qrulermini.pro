QT       += core gui

TARGET = qrulermini
TEMPLATE = app


SOURCES += main.cpp\
	ruler.cpp

HEADERS  += ruler.h

FORMS    += ruler.ui

win32:RC_FILE      = qrulermini.rc
