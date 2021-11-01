SUBDIRS = $(wildcard [01][0-9]_*)

.PHONY: subdirs $(SUBDIRS) clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@for i in $(SUBDIRS); do \
	    $(MAKE) -C $$i $@; \
	done
