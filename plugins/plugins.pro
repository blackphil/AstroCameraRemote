TEMPLATE = subdirs

SUBDIRS += fits \
    PluginTest

PluginTest.depends = fits
