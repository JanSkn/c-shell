<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/janskn/c-shell">
    <img src="src/images/logo.jpg" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">C Shell</h3>

  <p align="center">
    A simple shell written in C.
    <br />
    <a href="https://github.com/janskn/c-shell"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/janskn/c-shell/issues">Report Bug</a>
    ·
    <a href="https://github.com/janskn/c-shell/issues">Request Feature</a>
  </p>
</div>



<!-- ABOUT THE PROJECT -->
## About The Project

This C Shell can execute programs in the foreground or background (own programs or cd, ls, ...), wait for processes and use one pipe.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

This project was built with C

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Installation

_Follow these steps to run the project._

1. Clone the repo
   ```sh
   git clone https://github.com/janskn/c-shell.git
   ```
2. Install the build-essential package
   ```sh
   sudo apt install build-essential
   ```
   This installs a bunch of new packages including gcc, g++ and make

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Navigate to the path of the shell.c file.
<br />
Enter
```sh
   gcc -o shell shell.c
```
and afterwards
```sh
   ./shell
```
Or enter
```sh
   make run
```

Once you did this you can simply enter all the commands you want to get executed in the terminal.
Stop the program by typing exit.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
