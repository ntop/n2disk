# User's Guide (VitePress, see doc/Makefile)
.PHONY: docs docs-serve

docs:
	$(MAKE) -C doc dist

docs-serve:
	$(MAKE) -C doc serve
