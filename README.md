##Dependencies for all platforms:

make, gcc, libgtk3.0-dev, sqlite3-dev and codependencies

Dependency downloads:
* [Gtk3+](http://www.gtk.org/download/win32.php) (Make sure to download the All-in-one package)
* [SQLite](http://www.sqlite.org/download.html)


Extra dependencies for Windows:
* [MinGW32](http://www.mingw.org/)

**The current version of this program was tested using 32-bit Gtk+ 3.6.4, SQLite 3 and MinGW32 1.0. Other versions of these libraries have not been tested and are not gaurenteed to work!**

##Installation Instructions  


Linux:

	1. Install dependencies
	2. make

Windows:

	1. Install dependencies
	2. Download and install MinGW from the URL above
		- Be sure that MinGW-gcc and make are selected in the installation dialog before finalizing the installation.
	3. Update PATH
		- Properties of Computer -> Advanced Settings -> Environment Variables
		- Select "Path" and click edit
		- Add C:\path_you_installed_to\MinGW32\bin and C:\path_you_installed_to\gtk_win32\bin
	4. make
