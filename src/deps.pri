for(dep, DEPENDENCY_LIBRARIES) {
	src.depends += $${DESTDIR}/lib/lib$${dep}
	LIBS += -L$${DESTDIR}/lib/lib$${dep} -l$${dep}
	INCLUDEPATH += $${DESTDIR}/lib/lib$${dep}/src
	DEPENDPATH += $${DESTDIR}/lib/lib$${dep}
	win32: PRE_TARGETDEPS += $${DESTDIR}/lib/lib$${dep}/lib$${dep}.dll
	else: PRE_TARGETDEPS += $${DESTDIR}/lib/lib$${dep}/lib$${dep}.so
}
