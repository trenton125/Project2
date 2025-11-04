**Running the Game Search Engine**

**Set up**
	
To set up the game search engine, first build the program in order to fetch
the needed files from the SFML github page. Then, navigate through the cmake-build-debug and
to cmake-build-debug/_deps/sfml-build/lib and move sfml-graphics-d-2.dll, sfml-system-d-2.dll, and sfml-window-d-2.dll into the cmake-build-debug,
**outside of any other folder.** After that, run the program and you will be greeted with the window!

**Navigating the Interface**

Upon opening the program, you will be greeted with build time data in order to display the time it takes
to fully construct each data structure. To the right of that, there will be a slider that will let you choose
which data structure to use for searching. Under that are three buttons: View Data Structure Info, Search Game, and Add User
Rating. Clicking on View Data Structure Info will show some general information about the selected data structure, including build time,
search time complexity, and advantages. If you click Search Game, you may search for any game that you wish using the selected
data structure of your choice. On this page you can view information about the game including the platform, IGN rating, genre, and your own
personal rating! Finally, clicking the Add User Rating will allow you to enter your own rating for the game of your choice.