
s_messanger - README
=================================

Author: Sanjib Das
Developed in: Visual Studio 2022
Language: C++ (Windows Sockets API)
Port: 10000

Description
-----------
This project implements a multi-client chat server and client using TCP sockets. It supports basic chat, private messaging (via @username), and a command to list online users (#list).

S_messanger/
│
├── ChatServer/
│   ├── ChatServer/
│       ├── ChatServer.vcxproj
│       ├── ChatServer.vcxproj.filters
│       ├── ChatServer.vcxproj.user   
│       ├── main.cpp
│       ├── Server.cpp
│       └── Server.h
│   ├── ChatServer.sln

├── ChatClient/
│   ├── ChatClient/
│        ├── ChatClient.vcxproj
│        ├── ChatClient.vcxproj.filters
│        ├── ChatClient.vcxproj.user   
│        ├── main.cpp
│        ├── Client.cpp
│        └── Client.h
│   ├── ChatClient.sln
├── bin/
│   ├── ChatServer.exe
│   └── ChatClient.exe
├──Documentation/
    ├── Readme.txt



Requirements
------------
- Visual Studio 2022 or later
- Windows OS (Tested on windows 10)
- Enable C++17 or later
- WinSock library (already included with Windows SDK)

How to Compile and Run
----------------------
Build using Visual Studio

 1. Open `ChatServer.sln` and build the solution(Ctrl+Shift+B)in release mode.
 2. Open `ChatClient.sln` and build the solution(ctrl+Sift+B)in release mode.
 3. This will generate `.exe` files in:
     - `ChatServer/x64/Release/ChatServer.exe`
     - `ChatClient/x64/Release/ChatClient.exe`
 4. Run the server executable (Server.exe) first .
 5. Then run multiple client executables (Client.exe) to connect to the server.
 6. Clients must enter their name after connecting.

 Or directly run from pre-built binaries:
   1. Navigate to the given `bin/` folder.
   2. Run the server executable (Server.exe) first.
   3. Then run multiple client executables (Client.exe) to connect to the server.
   
Note: To run the executable, you can either:
Double-click the .exe file from the bin/ folder, or
Open Command Prompt, navigate to the folder, and run it using the command (e.g., ChatServer.exe).



Commands Available
------------------
- General messages are broadcast to all users.
- `@username <message>`: Send private message to a user.
- `#list`: Show list of online users.
- `#exit`: Disconnect from server.


Notes
-----
- All messages are handled concurrently using multithreading.
- The server can accept multiple clients and handles disconnects cleanly.

Developed By
------------
Sanjib Das
