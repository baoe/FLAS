# FLAGS
CXXFLAGS = -fopenmp#-Wall  # put compiler settings here
# put linker settings here
CXX	= g++

RM  	= rm -f 
MV	= mv
CP	= cp

all:FLAS preread bronkerboschadj addm42MECATm4 separatereads modifym4file2 filter full_trim write_corrected2 bin instbin clean


preread:src/preread.o
	$(CXX) src/preread.o -o preread

preread.o:src/preread.cpp
	$(CXX) -c src/preread.cpp

FLAS:src/FLAS.cpp src/combinecliques2.cpp
	$(CXX) src/FLAS.cpp src/combinecliques2.cpp -o FLAS -mcmodel=large -O

bronkerboschadj:src/bronkerboschadj.cpp
	$(CXX) src/bronkerboschadj.cpp -o bronkerboschadj  -mcmodel=large -O


addm42MECATm4:src/addm42MECATm4.o
	$(CXX) src/addm42MECATm4.o -o addm42MECATm4

addm42MECATm4.o:src/addm42MECATm4.cpp
	$(CXX) -c src/addm42MECATm4.cpp

separatereads:src/separatereads.o
	$(CXX) src/separatereads.o -o separatereads

separatereads.o:src/separatereads.cpp
	$(CXX) -c src/separatereads.cpp

modifym4file2:src/modifym4file2.o
	$(CXX) src/modifym4file2.o -o modifym4file2

modifym4file2.o:src/modifym4file2.cpp
	$(CXX) -c src/modifym4file2.cpp

filter:src/filter.o
	$(CXX) src/filter.o -o filter

filter.o:src/filter.cpp
	$(CXX) -c src/filter.cpp

full_trim:src/full_trim.o
	$(CXX) src/full_trim.o -o full_trim

full_trim.o:src/full_trim.cpp
	$(CXX) -c src/full_trim.cpp

write_corrected2:src/write_corrected2.o
	$(CXX) src/write_corrected2.o -o write_corrected2

write_corrected2.o:src/write_corrected2.cpp
	$(CXX) -c src/write_corrected2.cpp

bin:		
	mkdir bin

instbin:
	$(MV) FLAS preread bronkerboschadj addm42MECATm4 separatereads modifym4file2 filter full_trim write_corrected2 bin

clean:
	$(RM) src/FLAS.o src/combinecliques2.o src/preread.o src/bronkerboschadj.o src/addm42MECATm4.o  src/separatereads.o  src/modifym4file2.o  src/filter.o  src/full_trim.o src/write_corrected2.o

purge:		clean
	$(RM) -r bin/
