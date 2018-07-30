
TEMPLATE = subdirs

SUBDIRS += app
SUBDIRS += plugins

app.depends += plugins


