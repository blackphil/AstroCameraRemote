include($${top_srcdir}/common_pre.pri)

TEMPLATE = subdirs

SUBDIRS += AstroCameraRemote \
    AstroImageStacker \
    CameraSimulator


include($${top_srcdir}/common_post.pri)
