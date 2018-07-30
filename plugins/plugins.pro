TEMPLATE = subdirs

SUBDIRS += fits \
    PluginManager

fits.depends += PluginManager
