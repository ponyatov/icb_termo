FIG = doc/00.svg doc/01.svg

.PHONY: doc
doc: $(FIG)
doc/%.png: doc/%.dot
	dot -Tpng -o $@ $<
doc/%.svg: doc/%.dot
	dot -Tsvg -o $@ $<
