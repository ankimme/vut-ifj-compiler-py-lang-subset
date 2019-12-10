doc.pdf: doc.tex
	latex doc.tex
	dvips -t a4 doc.dvi
	ps2pdf doc.ps

clean:
	rm -f doc.aux doc.out doc.log doc.dvi doc.ps doc.pdf
