Dependencies for all platforms:

libgtk3.0-dev, sqlite3-dev and codependencies


Extra dependencies for Windows:

MinGW32 (http://www.mingw.org/)

-------------------------------
|  Installation Instructions  |
-------------------------------

Linux:

	1) Install dependencies
	2) make

Windows:
	1) Install dependencies
	2) Download and install MinGW from the URL above
	3) Update PATH
		- Properties of Computer -> Advanced Settings -> Environment Variables
		- Select "Path" and click edit
		- Add C:\path_you_installed_to\MinGW32\bin and C:\path_you_installed_to\gtk_win32\bin
	4) make
