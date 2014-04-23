This repository contains all the sotfware that was developed by the ground station team of the X-CubeSat. More informations about the project at http://xcubesat.wordpress.com/

- Requirements

    The current version of X-CubeSat Controler works only in POSIX compliant operating sy(-)s, like GNU/Linux, FreeBSD, etc, but future versions will work also in non POSIX compliant operating sy(-)s like Windows. You can find below the libraries and software needed to compile and run this version of XCubeSat Controler. The versions of the libraries and software below were tested, try other versions at your own risk.

    - g++ version 4.7.2
    - GNU Make 3.81
    - Git version 1.7
    - GTK+3 
    - gtkmm version 3.0 or above
    - SQLite version 3.7.13
    - SQLite Wrapped version 1.3.1
    - Gpredict modified version that can be found in the following address https://github.com/tlgimenes/gpredict

- Compiling and Installing

    The installation instructions will cover the compiling and installing phases. It will only shows how to install in the Debian GNU/Linux distribution but the same method should work in Ubuntu, Linux Mint and other Debian based distributions.

    You can install this program in any directory that you want. For doing it 

    (-) Installing dependencies:

    Open a terminal and type
    
    $ sudo apt-get install -y g++

    $ sudo apt-get install -y make

    $ sudo apt-get install -y git

    $ sudo apt-get install -y libgtkmm-3.0-1 libgtkmm-3.0-dev

    $ sudo apt-get install -y sqlite3

    $ wget http://www.alhem.net/project/sqlite/sqlitewrapped-1.3.1.tar.gz

    $ tar -zxvf sqlitewrapped-1.3.1.tar.gz

    $ cd sqlitewrapped-1.3.1/

    $ make && make install

    $ cd ..

    $ git clone https://github.com/tlgimenes/gpredict.git 

    $ cd gpredict

    $ make && make install

    $ ./src/gpredict

    Open a new terminal and type

    $ git clone https://github.com/tlgimenes/X-CubeSat

    $ $ cd X-CubeSat 

    $ make 

    $ cd dataBase 

    $ sqlite3 XCubeSat_Controler.db 

    .read tables.sql 

    .q 

    $ cd .. 

    $ ./XCubeSat\_ Controler

    And voilà ! Here you are running XCubeSat_Controler \o/.

    For more informations read the UserGuide.pdf file provided with this software in the doc folder.

TODO:
    (-) Improove commands change with the modem
