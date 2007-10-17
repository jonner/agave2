bin_PROGRAMS=agave

noinst_LTLIBRARIES=libagavewidgets.la libagavecore.la
libagavecore_la_SOURCES = \
color.h \
color.cc \
i-scheme.h \
scheme-manager.h \
scheme-manager.cc \
colormodel.h \
colormodel.cc \
color-relation.h \
color-relation.cc

libagavewidgets_la_SOURCES = \
swatch.h \
swatch.cc \
colorscale.h \
colorscale.cc \
color-edit-box.h \
color-edit-box.cc \
color-scheme-box.h \
color-scheme-box.cc \
scheme-combo-box.h \
scheme-combo-box.cc \
color-wheel.h \
color-wheel.cc

agave_SOURCES = \
agave.cc \
application.h \
application.cc \
application-window.h \
application-window.cc \
about-dialog.h \
about-dialog.cc

AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
agave_LDADD=$(AGAVE_LIBS) libagavewidgets.la libagavecore.la

doc : Doxyfile.in $(libagavewidgets_la_SOURCES) $(libagavecore_la_SOURCES)
	doxygen Doxyfile

doc-clean :
	rm -rf doc

.PHONY: doc doc-clean
