if HAVE_DOXYGEN
OUTPUT_DIRECTORY = $(top_srcdir)/doc/html

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: html
clean-local:
	rm -rf $(OUTPUT_DIRECTORY)

html: doxyfile.stamp
endif