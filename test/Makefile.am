noinst_PROGRAMS=test-swatch test-colorscale test-all test-ui test-relations test-wheel

#test_scheme_SOURCES = test-scheme.cc
#test_scheme_LDADD = $(AGAVE_LIBS) ../src/libagavecore.la ../src/libagavewidgets.la

test_swatch_SOURCES = test-swatch.cc
test_swatch_LDADD = $(UI_DEPS_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_colorscale_SOURCES = test-colorscale.cc
test_colorscale_LDADD=$(UI_DEPS_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_all_SOURCES = test-all.cc
test_all_LDADD=$(UI_DEPS_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_ui_SOURCES = test-ui.cc
test_ui_LDADD=$(UI_DEPS_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_wheel_SOURCES = test-wheel.cc
test_wheel_LDADD=$(UI_DEPS_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

test_relations_SOURCES = test-relations.cc
test_relations_LDADD=$(UI_DEPS_LIBS) ../src/libagavewidgets.la ../src/libagavecore.la

INCLUDES=-I$(top_srcdir)/src
AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(UI_DEPS_CFLAGS)
