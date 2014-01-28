Developing CruceGame on Microsoft Windows
=======

Although the development of _CruceGame_ makes heavy use of Linux/Unix tools, 
developing on MS Windows is also possible using 
[Cygwin](http://cygwin.com/install.html).

You will use the setup of `Cygwin` to install all the necessary tools, including
the compiler (`gcc`), `autotools` and `git`. 

##Step 1 - Download Cygwin

Download the [Cygwin setup](http://cygwin.com/setup-x86.exe). The x86 version
is ok even if you are runnig 64-bit Windows.

##Step 2 - Start the Installation

Start the setup, and select `Install from Internet`, select the installation 
folder and the a mirror near you.

##Step 3 - Select the Required Packages

After selecting the mirror, `Cygwin` will display a list of packages. There are 
a lot to choose from and now you will have to carefully select those needed 
by this project (some are already selected to be installed):

    Base/gzip
    Base/tar 
    Devel/automake1.14
    Devel/autoconf2.1
    Devel/gcc-core
    Devel/gcc-g++
    Devel/libgcc1
    Devel/gdb
    Devel/git
    Devel/gettext
    Devel/gettext-devel
    Devel/intltool
    Devel/libiconv
    Devel/libncurses-devel
    Devel/libncursesw-devel
    Devel/libtool
    Devel/make
    Devel/pkg-config
    Editors/mc
    Editors/nano
    Libs/libglib2.0-devel
    Utils/ncurses
    Web/wget
    Web/openssh

Cygwin will then find dependencies for the selected packages, which also need
to be installed. The installation of `Cygwin` will require ~ 600 MB on disk.

##Step 4 - Start Cygwin

Now that the `Cygwin` installation is complete, you can start the 
`Cygwin Terminal`. It is a shell, as those typically found on Linux/Unix 
systems. 

One important thing to remember is how it maps you Windows drives: if you want
to change the current folder to `D:\`, you have to type `cd /cygdrive/D/`.

Also, `~` is the local home folder, e.g. `c:\cygwin\home\Dani\`.

##Step 5 - Install Cutter

After installing `Cygwin`, you need to install `Cutter`. This is done by 
downloading its source code and compiling it via `Cygwin`.

####Create a folder of disk where the source files will be downloaded to

    mkdir ~/Downloads/
    cd ~/Downloads/

####Download Cutter Source Files

    wget http://downloads.sourceforge.net/project/cutter/cutter/1.2.2/cutter-1.2.2.tar.gz

####Unpack Cutter Source Files

    tar -xf cutter-1.2.2.tar.gz
    cd cutter-1.2.2

####Compile & Install Cutter

    ./configure --prefix=/usr/
    make
    make install
    
####Setup git public key

    mkdir ~/.ssh (if it does not already exist)
    cd ~/.ssh
    ssh-keygen -t rsa -C "your_email@example.com"

Open id_rsa.pub

    notepad id_rsa.pub
    
Copy the content of the file and add it to [SSH Keys](https://github.com/settings/ssh) of your GitHub profile.

####Create a fork of CruceGame from GitHub
    
####Checkout CruceGame

    cd ~
    git clone git@github.com:YOU/CruceGame.git
    cd CruceGame/
    
Select the `develop` branch

    git checkout develop

####Compile CruceGame for the first time

    ./autogen.sh
    ./configure
    make
    
###Test CruceGame for the first time

    make check