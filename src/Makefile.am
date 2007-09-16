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
colorscale.h \
colorscale.cc \
colormodel.h \
colormodel.cc \
color-relation.h \
color-relation.cc

agave_SOURCES = \
agave.cc \
application.h \
application.cc \
application-window.h \
application-window.cc

AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
agave_LDADD=$(AGAVE_LIBS) libagavecore.la libagavewidgets.la

doc : Doxyfile.in $(libagavewidgets_la_SOURCES) $(libagavecore_la_SOURCES)
	doxygen Doxyfile

doc-clean :
	rm -rf doc

.PHONY: doc doc-clean
