include ../Makefile.common
.PHONY: all plugin_fluid clean cleanall


# default target
all: plugin_fluid




# VR Juggler plugin
plugin_fluid:
	cd src && \
	$(MAKE) -C fluid && \
	mkdir -p ../lib/ && \
	cp fluid/liblifespacefluid.a \
	  ../lib/liblifespacefluid.a && \
	for dir in `find fluid/ -type d` ; do \
	  mkdir -p "../include/lifespace/plugins/$$dir" ; \
	done && \
	for file in `find fluid.hpp fluid/ -name '*.hpp'` ; \
        do \
	  cp "$$file" "../include/lifespace/plugins/$$file" ; \
	done
	@echo ""
	@echo "  The plugin has been created. Installation:"
	@echo "    cp -R include/lifespace/plugins \$$LIFESPACE_BASE_DIR/include/lifespace"
	@echo "    cp -R lib/liblifespacefluid.a \$$LIFESPACE_BASE_DIR/lib"
	@echo ""


# delete temporary files
clean:
	$(MAKE) -C src/fluid clean


# delete all generated files
cleanall:
	$(MAKE) -C src/fluid clean
	rm -rf lib include refman_user refman_devel
