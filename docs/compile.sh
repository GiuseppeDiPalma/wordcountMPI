#!/usr/bin/env bash

pandoc --listing \
        -H float.tex \
		-H section.tex \
		-H listing.tex \
        README.md -o GiuseppeDiPalma_WordCount.pdf