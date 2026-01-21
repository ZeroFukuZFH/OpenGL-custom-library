# **DEVLOG no.1**

This is my first take on a graphics engine library, for now it is capable only for 3D graphics.
I will fine tune it later for also 2D graphics but I am still in the process of learning
OpenGL as a whole.

## Why I made this library

I originally planned to simulate physics and space (shoutout to kavan for his 
video titled "simulating a black hole in C++" which largely contributed to my 
passion for this project) and in general just have a low-level graphics engine
on my hands to use for future projects. this was also a major level-up to my C
programming skills as this was primarily coded in C (not C++), the GLM library
was handmade in C since it does not provide support for C. only the GLFW and 
glad dependencies are written in C++ as i have no control of them and the 
vertex / fragment shaders because i don't think i can force C code to functions that's
meant to parse C++ code. luckily, actual implementation code can be written with
just C so that made it possible and easier for me to just not learn C++ (god i hate
that language so much with a passion)

## How to use

on vscode : 

open main.c (in app folder)

ctrl + shift + b (windows)
cmd + shift + b (mac)

commands : 

cd app 
./main 

