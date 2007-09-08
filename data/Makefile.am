# desktop files
desktopdir = $(datadir)/applications
desktop_in_files = agave.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

# gconf schemas
schemasdir = $(GCONF_SCHEMA_FILE_DIR)
schemas_in_files = agave.schemas.in
schemas_DATA = $(schemas_in_files:.schemas.in=.schemas)

@INTLTOOL_SCHEMAS_RULE@

if GCONF_SCHEMAS_INSTALL
install-data-local: $(schemas_DATA)
	if test -z "$(DESTDIR)" ; then \
	for p in $^ ; do \
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) \
	$(GCONFTOOL) --makefile-install-rule $$p >&1 > /dev/null; \
	done \
	fi
endif #GCONF_SCHEMAS_INSTALL

EXTRA_DIST = ${desktop_in_files} ${schemas_in_files}

CLEANFILES = ${desktop_DATA} ${schemas_DATA}
