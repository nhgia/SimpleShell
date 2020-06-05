# SimpleShell
> CS333 - Introduction to Operating Systems | Ho Chi Minh University of Science - VNU.

*Spring 2020 Course*

**PROJECT #2**: Build a simple shell on Linux: Simple commands with child processes, IO redirect &amp; pipe.

#### Author:
- 1751064 - Nguyen Hoang Gia 
- 1751063 - Pham Bao Duy


### Prerequisite
- Ubuntu / Linux. (Recommended Ubuntu 18.04)
- Download or clone the project.
- Install essential components by running:
```shell
sudo apt-get install build-essential
sudo apt-get install libreadline-dev
```
- Change directory to the project folder.

### Install: Compile the C file into output and runnable
- Simply by running
```shell
make
```

### Some commands for the SimpleShell that you can try
List sub-folder & sub-item
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

### Main feature
#### Check history of run commands
- Using double exclamation mark "!!" for checking history
```bash
!!
```
#### Concurrence commands
- Add an ampersand mark after the command with whitespace required.
```bash
ping -i 5 -w 10 -W 10 google.com &
```
#### To be updated
