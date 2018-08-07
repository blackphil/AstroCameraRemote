include($${top_srcdir}/common_pre.pri)

TEMPLATE = aux

INSTALLER = setup

SETUP_PACKAGES = $${top_builddir}/packages

INPUT = $$PWD/config/config.xml $$SETUP_PACKAGES
setup.input = INPUT
setup.output = $$INSTALLER

setup.commands += cp -rv $$PWD/packages/* $$SETUP_PACKAGES $$escape_expand(\\n\\t)
setup.commands += $$(QTDIR)/bin/windeployqt --compiler-runtime $$INSTALL_DEST_DIR $$escape_expand(\\n\\t)
setup.commands += $$(QT_IFW_ROOT)/bin/binarycreator -c $$PWD/config/config.xml -p $$SETUP_PACKAGES Setup $$escape_expand(\\n\\t)

setup.CONFIG += target_predeps no_link combine


OTHER_FILES = README

DISTFILES += \
    packages/org.AstroImage.app.AstroCameraRemote/meta/package.xml \
    packages/org.AstroImage.app.AstroCameraRemote/meta/installscript.qs \
    packages/org.AstroImage.app.AstroCameraRemote/meta/license.txt

include($${top_srcdir}/common_post.pri)

