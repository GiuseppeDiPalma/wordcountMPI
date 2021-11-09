#!/usr/bin/env bash

pandoc --listing \
        -H float.tex \
		-H section.tex \
		-H listing.tex \
        --number-sections \
        README.md -o GiuseppeDiPalma_WordCount.pdf