bin_PROGRAMS=agave

HEADER_FILES = color.h scheme.h i-scheme-generator.h scheme-generators.h application.h application-window.h swatch.h
SOURCE_FILES = agave.cc color.cc scheme.cc scheme-generators.cc application.cc application-window.cc swatch.cc
agave_SOURCES = $(HEADER_FILES) $(SOURCE_FILES)

AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
agave_LDADD=$(AGAVE_LIBS)
