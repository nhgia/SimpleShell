# SimpleShell
> CS333 - Introduction to Operating Systems | Ho Chi Minh University of Science - VNU.

*Spring 2020 Course*

**PROJECT #2**: Build a simple shell on Linux: Simple commands with child processes, IO redirect &amp; pipe.

*(Checkout the #1 project [here](https://github.com/nhgia/RandomNumber-LinuxKernelModule))*

#### Author:
- 1751064 - Nguyen Hoang Gia 
- 1751063 - Pham Bao Duy


### Prerequisite
- Ubuntu / Linux. (Recommended Ubuntu 18.04)
- Download or clone the project.
- Install essential components by running:
```shell
sudo apt-get install build-essential
```
- Change directory to the project folder.

### Install: Compile the C file into output and runnable
- Compile through Makefile, run
```shell
make
```
- Run the shell
```shell
./simple-shell
```


### SimpleShell commands and features
#### 1. Single command
- List sub-folder & sub-item
```bash
ls
```
- Current directory
```bash
pwd
```
- Ping website
```bash
ping -i 5 -w 10 -W 10 google.com
```
- ...

#### 2. Check history of run commands
- Using double exclamation mark "!!" for checking history
```bash
!!
```

#### 3. Concurrent commands
- Add an ampersand mark after the command with whitespace required.
```bash
ping -i 5 -w 10 -W 10 google.com &
```

#### 4. I/O redirection
- Redirect the output of a command to a file and input from a file to a command.
```bash
ls > myFile.txt
cat < myFile.txt
```

#### 5. Communication via a Pipe
- Output of one command to serve as input to another using a pipe.
- For example: list sub-folder and sub-item in current directory, just display which has character "a" in the name.
```bash
ls | grep a
```
