<img src="pixel.jpg" width=1000>

# The pixel Library

This is a **single file, object oriented** library to more easily 
create *fast* **graphical C++** applications that abstracts the 
creation of windows and makes using opengl much easier. It
currently only supports windows, as it uses the **Win32 API**, but
linux support *may* come in the future. 

# Compiling

This repository contains the **pixel.hpp** header file, which 
should be the only thing needed, as it is a single file solution.
The **'opengl32.lib'** and **'gdiplus.lib'** files must be added
to your linker's input for it to link properly.

Additionally, a Visual Studio solution is provided with some
**demos** to test the library's functionality, to use them simply
open the solution on **Visual Studio** and select the proper demo
file by editing it's properties and making it a **'C/C++ source
file"**.

# Features

This **library** provides easy creation of **windows** with the use of
the `Window` class. When creating a window, the user must specify
a **cavas size** and **scale**, window **position** and tittle, **drawing mode** 
and whether the user wants a **fullscreen** window and **vsync** enabled.

Serveral **drawing routines** are included, including lines, circles,
rectangles, and triangles. The user may specify a **color** and **aplha** 
values to draw them with.

The window must be updated in a loop for this library to work properly,
however, this is the only function that **must** be called periodically 
for the window to work. This update is triggered by the `Window::Update()` 
function, which updates the **user input**, **polls system events** and 
pushes the **buffer** into view.

The user may interogate the **mouse position** and **keyboard state** 
with the provided `Window::MousePos()` and `Window::KeyboardKey()` functions.

**FULL README COMMING SOON**
