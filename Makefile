all: main.exe

main.exe: main.o libAnaObject.so\
	AnaAlgorithm.o AnaBuffer.o\
	libAnaOptionParser.so
	g++ main.o AnaAlgorithm.o AnaBuffer.o -o main.exe `root-config --libs` `root-config --cflags` -lAnaObject -lAnaOptionParser -L. -I. 

main.o: main.cc
	g++ -c `root-config --cflags` `root-config --libs` -o main.o main.cc

libAnaObject.so: AnaObject.hh AnaObject.cc LinkDef.h
	rootcint -f Dict.cxx -c AnaObject.hh LinkDef.h
	g++ -shared -fPIC -o libAnaObject.so `root-config --cflags` Dict.cxx AnaObject.cc

libAnaOptionParser.so: AnaOptionParser.hh AnaOptionParser.cc AnaOptionParser.hh
	g++ -shared -fPIC -o libAnaOptionParser.so `root-config --cflags` AnaOptionParser.cc

AnaAlgorithm.o: AnaAlgorithm.hh AnaAlgorithm.cc AnaObject.hh 
	g++ -c `root-config --cflags` `root-config --libs` -o AnaAlgorithm.o AnaAlgorithm.cc

AnaBuffer.o: AnaBuffer.hh AnaBuffer.cc AnaObject.hh AnaOptionParser.hh
	g++ -c `root-config --cflags` `root-config --libs` -o AnaBuffer.o AnaBuffer.cc


clean:
	rm *.o *.so
