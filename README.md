# 🎲 Distributed Processing Project

## 📌 Project Title: Two-Player Turn-Based Dice Game

Welcome! This is a simple but fun multiplayer game made for the **Distributed Processing** course.  
It’s a console-based, turn-by-turn game with dice logic, network communication, and strategy.

---

## 🎮 How It Works

🖥️ One machine runs the **server**.  
🎯 Two players connect from clients (can be same or different PCs).  
📡 They play in real-time over a local network (LAN).  
📦 No graphics — everything is done in the terminal.

---

## 📏 Game Rules

- 🎲 Each turn, a player rolls **2 dice**.
- ➕ If the dice are **different** → the player makes **2 moves**.
- ⚔️ Players can **only move their own pieces**.
- 💣 The game ends when a player moves all his pieces out of board.

---

## 🕹️ Controls

1. You'll see the board and your dice.
2. Enter the field number to **move FROM**.
3. Then enter the field number to **move TO**.
4. Want to skip? Type `0` (only allowed when no valid moves are left).

---

## 🚀 How to Run It

### 🛠️ Server

Open the Server.exe on one machine.

Wait until it says: Waiting for 2 players to connect...

### 🧑‍💻 Clients

Open Client.exe on two separate terminals or two different machines.

Make sure to change SERVER_IP in Client.cpp to the correct IP address of the server machine.

Compile and run the clients.

Important: All devices must be on the same local network (Wi-Fi or Ethernet).


## 🗂️ Project Structure

