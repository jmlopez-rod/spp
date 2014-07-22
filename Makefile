all: export

export:
	git archive --format zip --output spp.zip master
