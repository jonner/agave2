bin_PROGRAMS=agave2

noinst_LTLIBRARIES=libagavewidgets.la libagavecore.la
libagavecore_la_SOURCES = \
color.h \
color.cc \
i-scheme.h \
scheme-manager.h \
scheme-manager.cc \
color-model.h \
color-model.cc \
color-relation.h \
color-relation.cc \
color-set.h \
color-set.cc \
color-set-manager.h \
color-set-manager.cc

libagavecore_la_CXXFLAGS=$(CORE_DEPS_CFLAGS)
libagavecore_la_LIBADD=$(CORE_DEPS_LIBS)

libagavewidgets_la_SOURCES = \
i-color-view.h \
swatch.h \
swatch.cc \
color-scale.h \
color-scale.cc \
rgb-scale-set.h \
rgb-scale-set.cc \
hsv-scale-set.h \
hsv-scale-set.cc \
color-edit-box.h \
color-edit-box.cc \
color-group-box.h \
color-group-box.cc \
scheme-combo-box.h \
scheme-combo-box.cc \
color-wheel.h \
color-wheel.cc \
color-set-details-editor.h \
color-set-details-editor.cc

libagavewidgets_la_CXXFLAGS=$(UI_DEPS_CFLAGS)
libagavewidgets_la_LIBADD=$(UI_DEPS_LIBS)

agave2_SOURCES = \
agave.cc \
application.h \
application.cc \
application-window.h \
application-window.cc \
about-dialog.h \
about-dialog.cc

agave2_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"
agave2_CXXFLAGS=$(UI_DEPS_CFLAGS)
agave2_LDADD=$(UI_DEPS_LIBS) libagavewidgets.la libagavecore.la

doc : Doxyfile.in $(libagavewidgets_la_SOURCES) $(libagavecore_la_SOURCES)
	doxygen Doxyfile

doc-clean :
	rm -rf doc

.PHONY: doc doc-clean
