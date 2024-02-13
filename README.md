# Robot Abstraction in C

## About the Project

The program displays an animation of a robot represented by a triangle that moves around the board until it reaches the marker. The background layer is a grid consisting of empty squares, blocks and a marker. The robot tries to find its way to the marker through empty squares and blocks. Block positions have been predefined in the source code, marker position is randomised and robot position is taken as input from the command line.

## Usage

Compile the program using the gcc compiler.

`gcc main.c graphics.c`

Then execute the following command to start the execution:

`./a.out <robot-x-position> <robot-y-position> <robot-direction> | java -jar drawapp-2.0.jar`

An example is `./a.out 2 3 north | java -jar drawapp-2.0.jar`

## License

Distributed under the MIT License. See LICENSE.md for more information.
