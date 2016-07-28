CXX = g++
CXXFLAGS = -std=c++11 -Wall -O -g
LDFLAGS=
OBJECTS = main.o Deck.o Card.o HumanPlayer.o Player.o ComputerPlayer.o Game.o
EXEC = exec

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${LDFLAGS}

main.o : main.cpp Deck.o
Game.o : Game.cpp Game.h Deck.o ComputerPlayer.o HumanPlayer.o
Player.o : Player.cpp Player.h Card.o
ComputerPlayer.o : ComputerPlayer.cpp ComputerPlayer.h Player.o
HumanPlayer.o : HumanPlayer.cpp HumanPlayer.h Player.o
Deck.o : Deck.cpp Deck.h Card.o
Card.o : Card.cpp Card.h

clean :
	rm -rf ${OBJECTS} ${EXEC}
