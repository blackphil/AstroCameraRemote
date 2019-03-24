include($${top_srcdir}/common_pre.pri)

TEMPLATE = subdirs

SUBDIRS += app \
    AstroBase \
    BatchProcess \
    Setup \
    DataFormat \
    EasyExif
SUBDIRS += plugins

app.depends += AstroBase BatchProcess DataFormat EasyExif
app.depends += plugins

BatchProcess.depends += AstroBase plugins DataFormat EasyExif

plugins.depends += AstroBase DataFormat

DataFormat.depends += AstroBase

setup.depends += app AstroBase BatchProcess plugins DataFormat



include($${top_srcdir}/common_post.pri)

