Raspberry Pi OpenGL GLUT Benchmark

Needs freeglut software installation:

sudo apt-get install freeglut3
sudo apt-get install freeglut3-dev

My OpenGL benchmark runs via Windows and Linux. The latter is described in the following that also includes results. 

http://www.roylongbottom.org.uk/linux%20opengl%20benchmarks.htm  

NOTE: In 1992, I approved a request from  a Quality Engineer at  Canonical, to use this OpenGL benchmark in the testing framework of the Unity desktop software. 

See ogl1.c for compile command

The benchmark has six tests, as shown in the results, that are displayed (if visible) and saved in log file videogl.txt. 

Default ./videogl64 runs in full screen mode but can also has run time parameters to use other sizes and avoid other details, as in script file runit.sh, containing the following

export vblank_mode=0
./videogl64 Width 160, Height 120, NoEnd
./videogl64 Width 320, Height 240, NoHeading, NoEnd
./videogl64 Width 640, Height 480, NoHeading, NoEnd
./videogl64 Width 1024, Height 768, NoHeading, NoEnd
./videogl64 NoHeading

export vblank_mode=0 turns off Wait For Vertical Blank to enable > 60 FPS

Default running time is 5 Seonds per test. This can be increased by another parameter, such as:

./videogl64 Width 1024, Height 768, Seconds 20, NoHeading, NoEnd


OTHER FUNCTION - Reliability Tests 

A single test (1 to 6) can be run for extended periods as follows:

./videogl64 Wide 1024, High 768, Test 6, Minutes 20

or down to

./videogl64 W 1024 H 768 T 6 M 20

FPS speeds are logged and displayed every 30 seconds 



OTHER FUNCTION - Zoom and Rotation Speed

On running tests 5 or 6 for extended periods, Zoom in and out is provided  by up/down keys and rotation speeds via left/right keys.


LOG FILE - see videogl1.txt

An example log file is included, mainly to show configuration details obtained.

IMAGES USED

In order to run, the .bmp images must be in the same folder as the compiled videogl64.

Different anf larger .bmp image files can be used, but withthe same names. Following are results where 12 KB images were increased to 192 KB (64 x 64 pixels to 256 x 256), showing impact on textured tests.

Roy Longbottom
March 2017
