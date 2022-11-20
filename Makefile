obj=main.o scanner/scanner.o parser/parser.o semantic/semantic.o errorReporter/errorReporter.o

GraphCompiler.exe: $(obj)
	g++ -o GraphCompiler.exe $(obj)

main.o: scanner/scanner.h errorReporter/errorReporter.h parser/parser.h semantic/semantic.h
scanner/scanner.o: scanner/scanner.h
semantic/semantic.o: semantic/semantic.h
parser/parser.o: parser/parser.h semantic/semantic.h

.PHONY: clean
clean:
	-rm $(obj)