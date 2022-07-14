#
# Top Makefile
#

.PHONY: all clean

all:
	@$(MAKE) -C src all
	@$(MAKE) -C examples all
	@$(MAKE) -C workshop all

clean:
	@$(MAKE) -C src clean
	@$(MAKE) -C examples clean
	@$(MAKE) -C workshop clean

run:
	$(all)
	@$(MAKE) -C workshop run
