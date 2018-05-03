# Jonathan's Simple Operating System

## About

This is a simple, CLI-based operating system that I am building for fun and to learn about how operating systems work. 

## Getting Started

### Prerequisites

- Bash Shell
- Bochs
- Bochs SDL plugin
- NASM Assembler
- `build-essential`
- GCC (in build-essential)
- `genisoimage`

On Ubuntu, you can install the prerequisites with the following command:

```
sudo apt install build-essential nasm genisoimage bochs bochs-sdl
```

On other Linux systems, you may have to install the prequisite packages manually and/or compile Bochs with the SDL plugin (for example, the AUR does not have bochs-sdl). You can find [more information](http://bochs.sourceforge.net/doc/docbook/user/compiling.html#COMPILE-SDL) on it here. After extracting Bochs, you can compile it with SDL using the following commands.

```
$ ./configure --with-sdl
$ make
$ sudo make install
```

I have only tested this on Arch Linux and Ubuntu, however, so it's possible that set up problems may come up on other systems, such as Windows or OS X. Additionally, I have only ran this OS through Bochs on Bash, so this may not work on other shells.


### Running the OS

Use the following commands to run the OS

- To build the OS
```
$ make
```
or
```
$ make fresh
```
- To run (and build if necessary) the OS
```
$ make run
```
- To remove object and other intermediate files (highly recommended, not doing so has occasionally caused problems)
```
$ make clean
```

## Authors

* Jonathan Tong &mdash; [Jont828](https://github.com/Jont828)


## Errors

If you find an error (there are plenty) that's not already listed, please open a new [issue](https://github.com/Jont828/Jonathan-OS/issues).

## Contributing

If you would like to contribute new features/material or by fixing errors, please fork the repository, make your changes, and open a pull request.

## Acknowledgements

I would like to thank Erik Helin and Adam Renberg, the authors of [The Little Book About OS Development](http://littleosbook.github.io/), and Brandon Friesen, the author of [Bran's Kernel Development Tutorial](http://www.osdever.net/bkerndev/Docs/title.htm), whose tutorials I used to begin the project.

## License

This project is licensed under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html). Follow the link for more details.