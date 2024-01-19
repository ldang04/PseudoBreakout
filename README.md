# PseudoBreakout Project

The PseudoBreakout project is a simplified version of the popular arcade game "Breakout", which I completed for my Computer Science I (COSC-051-10) class 
at Georgetown University this summer (July 2021). This program includes collision detection and user-control features that I fully implemented using C++ , with
help from the [SFML](https://www.sfml-dev.org/tutorials/2.5/) library. 

## Running this program

In order to run this program on your computer, you must have SFML set up on your compiler of choice. You can find the instructions for that under the 
[Getting Started](https://www.sfml-dev.org/tutorials/2.5/) section on the SFML website. 

If you pull this full folder from github, you should be able to run the code by first entering the project on your command line (cd pseudoBreakoutv2)*, and then 
start the game by entering ./breakout **. 

*I'm not sure if the correct folder name would be pseudoBreakout or pseudoBreakoutv2 (the original version), so if the latter doesn't work, try entering 
"cd pseudoBreakout" instead. 

** You may want to re-compile the project on your own device by entering: 

g++ -g -std=c++20 -o breakout pseudoBreakoutv2.cpp -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system

into your command line. 

## Demo 

Since I am  unable to deploy this program onto Github Pages like my other projects, I have included (or at least tried to include)
a video of the game in action. 

[![PseudoBreakoutIMG](https://img.youtube.com/vi/ed4BAeXcuc0/0.jpg)](https://youtu.be/ed4BAeXcuc0)

(Click on the image to go to a linked YouTube demo)
