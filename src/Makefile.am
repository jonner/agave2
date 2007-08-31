bin_PROGRAMS=agave

noinst_LTLIBRARIES=libagavewidgets.la libagavecore.la
libagavecore_la_SOURCES = \
color.h \
color.cc \
scheme.h \
scheme.cc \
i-scheme-generator.h \
scheme-generators.h \
scheme-generators.cc

libagavewidgets_la_SOURCES = \
swatch.h \
swatch.cc \
agave-colorscale.h \
agave-colorscale.cc \
agave-colormodel.h \
agave-colormodel.cc

agave_SOURCES = \
agave.cc \
application.h \
application.cc \
application-window.h \
application-window.cc

AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
agave_LDADD=$(AGAVE_LIBS) libagavecore.la libagavewidgets.la
