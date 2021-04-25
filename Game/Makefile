all: server client


server:
	g++ -o server server.cpp Game.cpp Board.cpp Room.cpp Players.cpp

client:
	g++ -o client client.cpp

clean:
	rm -rf othello server client