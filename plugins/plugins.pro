include($${top_srcdir}/common_pre.pri)

TEMPLATE = subdirs

SUBDIRS += fits \
    PluginTest

PluginTest.depends = fits

include($${top_srcdir}/common_post.pri)
