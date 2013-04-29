## How To Use The Code ##

**Building the project:**

Run ```sudo ./buildall.sh``` to build and install the module as well as build the keyreader used to track keys from the module.

**Running the keyreader:**
```bash

cd keyreader/bin/     # Keyreader's output dir
./keyreader (tty|key) # To listen for different streams of logged keys coming from the module
```

**Uninstalling the module:**
```bash
cd module
sudo make uninstall
```

## Files ##

  * **keyreader** - *Directory that contains the user-level keyreader application*
    * **Makefile** - *Builds the keyreader application*
    * **keyreader.cpp** - *Contains the entire implementation of the keyreader application*
  * **module** - *Directory that contains the keylogger kernel module**
    * **Makefile** - *Builds and installs the keylogger kernel module*
    * **keylogger.c** - *Contains the primary implementation of the keylogger kernel module*
    * **keymap.h** - *Contains information to map keyboard key presses into readable strings*
  * **Cornwell_Lesnau_Report.pdf** - *Paper for the CSE 812 Spring 2013 Semester Project*
  * **README.md** - *Brief documentation in Markdown format to be displayed on GitHub*
  * **buildall.sh** - *Builds and installs keylogger module. Builds keyreader application*