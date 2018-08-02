#message(top_srcdir: $${top_srcdir})
#message(top_builddir: $${top_builddir})

#message("before loop DLL_DEPS ...")
for(DLL_DEP, DLL_DEPS) {
#    message(DLL_DEP: $$DLL_DEP)
#    message(DLL: $$basename(DLL_DEP))

    INCLUDEPATH += $${top_srcdir}/$$DLL_DEP

    build_pass:CONFIG(debug, debug|release) {
        LIBS += -L$${top_builddir}/$$DLL_DEP/debug $$basename(DLL_DEP).lib
    } else {
        LIBS += -L$${top_builddir}/$$DLL_DEP/release $$basename(DLL_DEP).lib
    }
}

#message("after loop DLL_DEPS ...")
