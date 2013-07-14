gc = g++ -Wall
all: IsomorphMatrices.o BasicMatrix.o PermVectorTrunk.o PermVectorTrunk.o Stats.o
					$(gc) -o amorph IsomorphMatrices.o BasicMatrix.o PermVectorTrunk.o Stats.o
IsomorphMatrices.o: Stats.o PermVectorTrunk.o IsomorphMatrices.cpp  
					$(gc) -c IsomorphMatrices.cpp
PermVectorTrunk.o:	PermVectorTrunk.cpp Stats.cpp
					$(gc) -c PermVectorTrunk.cpp
BasicMatrix.o:		Stats.o PermVectorTrunk.o BasicMatrix.cpp
					$(gc) -c BasicMatrix.cpp 
Stats.o:			Stats.cpp
					$(gc) -c Stats.cpp