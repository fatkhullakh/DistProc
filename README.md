README: Distributed Processing Project

Project Title: Two-Player Turn-Based Game (Dice and Board Logic)

This is a simple multiplayer board game made for the Distributed Processing course.

The game is turn-based, and two players play against each other over a network.
One player is White, the other is Black. They take turns rolling dice and moving their pieces on the board.

How It Works

The server runs on one computer.

Two players connect to the server using the client app (can be on the same or different machines).

The game is displayed in the console.

Game Rules (Simple Version)

Each turn, a player rolls 2 dice.

If both dice are different (e.g. 3 and 5), the player can make 2 moves.

If both dice are the same (e.g. 6 and 6), the player gets 4 moves.

The player can only move their own pieces.

The game ends when one player captures all of the other player's pieces.

Controls

During your turn:

You will see the board and your dice.

First, enter the field number you want to move from.

If it's not your piece, or it can't move, you will be told.

Then, enter the field number you want to move to.

To skip your turn (if no moves are possible), type 0.

How to Run

Server

Open the Server.exe on one machine.

Wait until it says: Waiting for 2 players to connect...

Client

Open Client.exe on two separate terminals or two different machines.

Make sure to change SERVER_IP in Client.cpp to the correct IP address of the server machine.

Compile and run the clients.

Important: All devices must be on the same local network (Wi-Fi or Ethernet).

Files Included

Server.cpp: Game server logic

Client.cpp: Player-side application

GameLogic.cpp/h: Game rules and logic functions

game_log.txt: File where all moves and events are recorded

Authors

Fatkhullakh Turakhonov (Index: 192066)
Mark Slipenkyi (Index: 196668)

Notes

Use Windows Firewall settings to allow TCP port 54000 for server.

If testing on one PC, use 127.0.0.1 as the server IP.

If using two machines, use the IPv4 address of the server PC.

Final Tips

Always run the server before the clients.

Make sure the client connects to the correct IP address.

You can quit the game by closing the window.

Good luck and have fun playing!
