noinst_PROGRAMS=test-scheme test-swatch

test_scheme_SOURCES = test-scheme.cc \
					  $(top_srcdir)/src/scheme.h \
					  $(top_srcdir)/src/scheme.cc \
					  $(top_srcdir)/src/i-scheme-generator.h \
					  $(top_srcdir)/src/scheme-generators.h \
					  $(top_srcdir)/src/scheme-generators.cc \
					  $(top_srcdir)/src/color.h \
					  $(top_srcdir)/src/color.cc

test_swatch_SOURCES = test-swatch.cc \
					  $(top_srcdir)/src/color.h \
					  $(top_srcdir)/src/color.cc \
					  $(top_srcdir)/src/swatch.h \
					  $(top_srcdir)/src/swatch.cc

INCLUDES=-I$(top_srcdir)/src
AM_CPPFLAGS = -DAGAVE_LOCALEDIR=\"${AGAVE_LOCALEDIR}\"

AM_CXXFLAGS=$(AGAVE_CFLAGS)
LDADD=$(AGAVE_LIBS)
