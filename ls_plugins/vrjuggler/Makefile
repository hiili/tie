include ../Makefile.common
.PHONY: all plugin_vrjuggler clean cleanall


# default target
all: plugin_vrjuggler




# VR Juggler plugin
plugin_vrjuggler:
	cd src && \
	$(MAKE) -C vrjuggler && \
	mkdir -p ../lib/ && \
	cp vrjuggler/liblifespacevrjuggler.a \
	  ../lib/liblifespacevrjuggler.a && \
	for dir in `find vrjuggler/ -type d` ; do \
	  mkdir -p "../include/lifespace/plugins/$$dir" ; \
	done && \
	for file in `find vrjuggler.hpp vrjuggler/ -name '*.hpp'` ; \
        do \
	  cp "$$file" "../include/lifespace/plugins/$$file" ; \
	done
	@echo ""
	@echo "  The plugin has been created. Installation:"
	@echo "    cp -R include/lifespace/plugins \$$LIFESPACE_BASE_DIR/include/lifespace"
	@echo "    cp -R lib/liblifespacevrjuggler.a \$$LIFESPACE_BASE_DIR/lib"
	@echo ""


# delete temporary files
clean:
	$(MAKE) -C src/vrjuggler clean


# delete all generated files
cleanall:
	$(MAKE) -C src/vrjuggler clean
	rm -rf lib include refman_user refman_devel
