include($${top_srcdir}/common_pre.pri)

TEMPLATE = subdirs

SUBDIRS += app \
    AstroBase \
    BatchProcess \
    Setup
SUBDIRS += plugins

app.depends += AstroBase BatchProcess
app.depends += plugins

BatchProcess.depends += AstroBase plugins

plugins.depends += AstroBase

setup.depends += app AstroBae BatchProcess plugins

include($${top_srcdir}/common_post.pri)

