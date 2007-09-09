noinst_PROGRAMS=test-scheme test-swatch test-colorscale test-all test-ui

test_scheme_SOURCES = test-scheme.cc
test_scheme_LDADD = $(AGAVE_LIBS) ../src/libagavecore.la ../src/libagavewidgets.la

test_swatch_SOURCES = test-swatch.cc
test_swatch_LDADD = $(AGAVE_LIBS) ../src/libagavecore.la ../src/libagavewidgets.la

test_colorscale_SOURCES = test-colorscale.cc
test_colorscale_LDADD=$(AGAVE_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_all_SOURCES = test-all.cc
test_all_LDADD=$(AGAVE_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_ui_SOURCES = test-ui.cc
test_ui_LDADD=$(AGAVE_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

INCLUDES=-I$(top_srcdir)/src
AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
