include($${top_srcdir}/common_pre.pri)

TEMPLATE = subdirs

SUBDIRS += FitsImagePlugin \
    PluginTest

PluginTest.depends = FitsImagePlugin

include($${top_srcdir}/common_post.pri)
