CXX = g++
CXXFLAGS = -g -std=c++14 -Werror=vla -Wall -MMD
#CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = sorcery
OBJECTS = display.o main.o suit.o game.o card.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
