ACLOCAL_AMFLAGS = --install -I m4

SUBDIRS = data include src po

doc_DATA = COPYING README

# these are included recursively by dist
EXTRA_DIST = doc packaging tools

# Always ensure README exists by copying from README.md
all-local: README

README: README.md
	cp -f README.md README
