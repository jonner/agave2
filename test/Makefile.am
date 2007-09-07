noinst_PROGRAMS=test-scheme test-swatch test-colorscale-rgb test-colorscale-hsv

test_scheme_SOURCES = test-scheme.cc
test_scheme_LDADD = $(AGAVE_LIBS) ../src/libagavecore.la ../src/libagavewidgets.la

test_swatch_SOURCES = test-swatch.cc
test_swatch_LDADD = $(AGAVE_LIBS) ../src/libagavecore.la ../src/libagavewidgets.la

test_colorscale_rgb_SOURCES = test-colorscale-rgb.cc
test_colorscale_rgb_LDADD=$(AGAVE_LIBS) ../src/libagavewidgets.la

test_colorscale_hsv_SOURCES = test-colorscale-hsv.cc
test_colorscale_hsv_LDADD=$(AGAVE_LIBS) ../src/libagavewidgets.la

INCLUDES=-I$(top_srcdir)/src
AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
