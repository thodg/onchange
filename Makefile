
SUBDIRS = onchange

all: ${SUBDIRS}
all: TARGET =

clean: ${SUBDIRS}
clean: TARGET = clean

${SUBDIRS}: %:
	${MAKE} -C build/$@ ${TARGET}

.PHONY: all clean ${SUBDIRS}
