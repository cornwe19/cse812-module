**Building the project:**

Run ```sudo ./build.sh``` to build and install the module as well as build the keyreader used to track keys from the module.

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
