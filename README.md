# pplectures2021
Jupyter notebooks for Parallel Programm (CS/JHU) Fall 2021

The tools we are going to use for lectures:

  * python -- interpreted programming language favored by CS-oriented data scientist and ML folks
  * conda -- package manager and configuration environment for Python
  * jupyter lab -- literate programming environment that mixes code and markdown
  * github -- cloud-based repository management for sharing stuff
  
To get here, we need to do the following:

### Install python:

For windows, I recommend using WSL (Windows Systems for Linux). I am running Ubuntu 20.04.2 LTS which comes with Python 3.8.

### Install conda:

Anaconda prefers that you download an installer from https://www.anaconda.com/products/individual
If you are using WSL, you need to download and run the Ubuntu installer
Create and activate a conda virtual environment

  * `conda create -n pp`
  * `conda activate pp`

#### Install the python packages

`conda install numba scikit-learn scikit-image jupyterlab pip`


Clone the github repository and move into directory

  * `git clone https://github.com/randalburns/pplectures2021.git`
  * `cd pplectures2021`

### Run jupyter lab

`jupyter lab`
