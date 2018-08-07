for(DLL_DEP, DLL_DEPS) {
    INCLUDEPATH += $${top_srcdir}/$$DLL_DEP

    build_pass:CONFIG(debug, debug|release) {
        LIBS += -L$${top_builddir}/$$DLL_DEP/debug $$basename(DLL_DEP).lib
    } else {
        LIBS += -L$${top_builddir}/$$DLL_DEP/release $$basename(DLL_DEP).lib
    }
}

for(INC_DEP, INC_DEPS) {
    INCLUDEPATH += $${top_srcdir}/$$INC_DEP
}


target.path += $$INSTALL_DEST_DIR

INSTALLS *= $$INSTALL_FILES

QMAKE_EXTRA_TARGETS += setup

