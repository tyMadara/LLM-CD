
LIB	= .
INCLUDE = .

CC	= g++
CPPFLAGS = -I$(INCLUDE) -O3 -Wall -std=c++11
#CPPFLAGS = -I$(INCLUDE) -O3 -Wall -std=c++11 -DDEBUG
SRCS  = main.cpp \
	instance.cpp \
	variable.cpp \
	parentset.cpp \
	ordering.cpp \
	localsearch.cpp \
	pivotresult.cpp \
	searchresult.cpp \
	population.cpp \
	resultregister.cpp \
	util.cpp \
	tabulist.cpp \
	movetabulist.cpp \
	swaptabulist.cpp \
	swapresult.cpp \
	fastpivotresult.cpp

OBJS  =	$(SRCS:.cpp=.o)

all:	$(OBJS)
	$(CC) $(CPPFLAGS) -o search $(OBJS)

clean:	;rm -f $(OBJS) \
	search \
	search.exe \
	search.exe.core \
	search.exe.stackdump

  
###
main.o:			instance.h localsearch.h resultregister.h util.h types.h
instance.o:		instance.h variable.h types.h
variable.o:		variable.h parentset.h
parentset.o:		parentset.h types.h
ordering.o:		ordering.h instance.h searchresult.h types.h
localsearch.o:		localsearch.h instance.h pivotresult.h searchresult.h population.h util.h movetabulist.h tabulist.h swaptabulist.h swapresult.h types.h
pivotresult.o:		ordering.h types.h
searchresult.o: 	searchresult.h types.h
population.o :		ordering.h instance.h localsearch.h types.h
resultregister.o:	types.h searchresult.h ordering.h
util.o:			types.h
tabulist.o: 		tabulist.h ordering.h
movetabulist.o: 	movetabulist.h ordering.h
swaptabulist.o:		swaptabulist.h ordering.h
swapresult.o:		swapresult.h types.h
fastpivotresult.o:	fastpivotresult.h ordering.h types.h
