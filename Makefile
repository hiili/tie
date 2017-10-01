include Makefile.common


# default target --------------------------------
all: lib test


# the tie library -------------------------------
lib:
	cd src && \
	$(MAKE) && \
	mkdir -p ../lib/ && \
	cp libtie.a ../lib/libtie.a && \
	for dir in `find ./ -type d` ; do \
	  mkdir -p "../include/tie/$$dir" ; \
	done && \
	for file in `find ./ -name '*.hpp'` ; do \
	  cp "$$file" "../include/tie/$$file" ; \
	done




### other targets
### ------------------------------------------------------------- ###

.PHONY: all clean cleantest cleanexamples cleanbin cleanall \
    lib \
    test \


# tests
test: lib
	$(MAKE) -C test


# remove temporary files from the src directory
clean:
	$(MAKE) -C src clean

# delete all tests
cleantest:
	$(MAKE) -C test clean

# delete all compiled libraries and their copied headers
cleanbin:
	rm -rf include/ lib/

# delete all derived files
cleanall: clean cleantest cleanbin
