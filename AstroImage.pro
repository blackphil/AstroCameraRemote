TEMPLATE = subdirs

SUBDIRS += app \
    AstroBase \
    BatchProcess
SUBDIRS += plugins

app.depends += AstroBase BatchProcess
app.depends += plugins

BatchProcess.depends += AstroBase plugins

plugins.depends += AstroBase


