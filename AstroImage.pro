include($${top_srcdir}/common_pre.pri)

TEMPLATE = subdirs

SUBDIRS += app \
    AstroBase \
    BatchProcess \
    Setup \
    DataFormat
SUBDIRS += plugins

app.depends += AstroBase BatchProcess DataFormat
app.depends += plugins

BatchProcess.depends += AstroBase plugins DataFormat

plugins.depends += AstroBase DataFormat

DataFormat.depends += AstroBase

setup.depends += app AstroBase BatchProcess plugins DataFormat



include($${top_srcdir}/common_post.pri)

