# cwushell
**Lab 1 - CS 470 - Operating Systems**\
Custom Shell Script written from scratch in C

## Available Commands

Command | Switches | Description
--------|----------|------------
systeminformation | [-p], [-m], [-o] | Prints information about the system
memoryinformation | [-s], [-u], [-S] | Prints information about the system memory
manual | none | Displays the manual
exit | [n] | exits the shell
history | [n] | prints the latest n commands in history
*command* | *parameters* | executes any existing shell command

## Running the Shell
This shell was built to run in a Linux environment. If the local machine does not run Linux, alternatives include Docker, virtual machines, and dual boots.\
If using Docker, you can download the Docker image I created that already has the lab installed or download any Linux distro image with GCC downloaded. Here are some recommended methods:
### My image
Minimal work: download image and run
- Pull with `docker pull zdehaan/cwushell:1.0`
- Run with `docker run -it zdehaan/cwushell:1.0`
- Once inside the container, start the script by running `./cwushell`
### Other images
Medium work: download image, import C file, compile, and run
- Pull with `docker pull szilardvajda/ubuntu_cs370_cs470_cs530:1.0`
- Run with `docker run -it szilardvajda/ubuntu_cs370_cs470_cs530`
- Create a file for the C program by running `touch cwushell.c` and open it by running `nano cwushell.c`
- Copy and paste the contents of `/src/cwushell.c` into the GNU Editor and save the changes
- Create the executable by running `gcc cwushell.c -o cwushell`
- Start the script by running `./cwushell`
### Creating an image
Most work: download image, download GCC, import C file, compile, run
- Start with a basic Ubuntu image by running `docker pull ubuntu:latest`
- Run it with `docker run ubuntu:latest`
- Once inside of the container, run `sudo apt install gcc` to download GCC or `sudo apt install build-essential` to download GCC as well as some libraries and G++
- Create a file for the C program by running `touch cwushell.c` and open it by running `nano cwushell.c`
- Copy and paste the contents of `/src/cwushell.c` into the GNU Editor and save the changes
- Create the executable by running `gcc cwushell.c -o cwushell`
- Start the script by running `./cwushell`


## To-Do:
* Implement command history navigation via the up and down arrow keys
* Implement command for making and opening a file
* Add some fun stuff?
