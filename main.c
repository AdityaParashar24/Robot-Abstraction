#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <string.h>
#include <time.h>
#include "main.h"
#include <ctype.h>

int main(int argc, char **argv) {
    // Seeding the pseudo random-number generator so that we get
    // different values everytime we run the rand() function
    srand(time(0));

    int window_size_x = 1000, window_size_y = 1000;
    setWindowSize(window_size_x, window_size_y);

    // Taking input from command line
    // The default values if the command line arguments are not given or user enters non-digit values
    int robot_initial_x = 6;
    int robot_initial_y = 5;
    char *robot_initial_direction_str = "south";

    if (argc == 4) {
        if (isdigit(*argv[1])) robot_initial_x = atoi(argv[1]);
        if (isdigit(*argv[2])) robot_initial_y = atoi(argv[2]);
        robot_initial_direction_str = argv[3];
    }

    struct robot arobot;
    init_robot(&arobot, robot_initial_x, robot_initial_y, robot_initial_direction_str);

    if (!check_if_input_valid(arobot)) return 0;

    // Setting background colour and drawing grid, blocks and marker
    int marker_x, marker_y;
    background();
    set_background_colour(window_size_x, window_size_y);
    setting_background(&marker_x, &marker_y);

    foreground();
    draw_robot(arobot.x_pos, arobot.y_pos, arobot.direction);

    // The loop keeps on running until the robot finds the marker
    while (!atMarker(arobot, marker_x, marker_y)) {
        sleep(150);
        forward(&arobot);
    }
    return 0;
}

void set_background_colour(int window_size_x, int window_size_y) {
    setColour(black);
    fillRect(0, 0, window_size_x, window_size_y);
}

void draw_columns(int end_x, int end_y) {
    for (int i = start_x; i <= end_x; i += width) {
        drawLine(i, start_y, i, end_y);
    }
}

void draw_rows(int end_x, int end_y) {
    for (int i = start_y; i <= end_y; i += width) {
        drawLine(start_x, i, end_x, i);
    }
}

int check_if_input_valid(struct robot arobot) {
    if (arobot.x_pos >= grid_size || arobot.y_pos >= grid_size) {
        printf("Marker positions exceed the grid size. Please enter a value from 0-%d\n", grid_size - 1);
        return 0;
    }
    if (!arobot.direction) {
        printf("Please enter a direction i.e. north/south/east/west\n");
        return 0;
    }
    return 1;
}

// Function to convert the string input passed in the
// command line to one of the constant integers representing a direction
int direction_datatype_conversion(char *robot_initial_direction_str) {
    if (!strncmp(robot_initial_direction_str, "north", sizeof(&robot_initial_direction_str))) return 1;
    if (!strncmp(robot_initial_direction_str, "east", sizeof(&robot_initial_direction_str))) return 2;
    if (!strncmp(robot_initial_direction_str, "south", sizeof(&robot_initial_direction_str))) return 3;
    if (!strncmp(robot_initial_direction_str, "west", sizeof(&robot_initial_direction_str))) return 4;
    return 0;
}

void init_robot(struct robot *arobot, int robot_initial_x, int robot_initial_y, char *robot_initial_direction_str) {
    arobot->x_pos = robot_initial_x;
    arobot->y_pos = robot_initial_y;
    arobot->direction = direction_datatype_conversion(robot_initial_direction_str);
}

// Drawing grid, blocks and marker
void setting_background(int *marker_x, int *marker_y) {
    setColour(white);
    draw_columns(end_x, end_y);

    draw_rows(end_x, end_y);

    int num_of_blocks = 4;
    int block_positions_x[] = {0, 3, 7, 8};
    int block_positions_y[] = {0, 9, 1, 5};
    draw_blocks(num_of_blocks, block_positions_x, block_positions_y);

    *marker_x = rand() % grid_size;
    *marker_y = rand() % grid_size;

    if (marker_coinciding_with_block(block_positions_x, block_positions_y, num_of_blocks, *marker_x, *marker_y)) {
        *marker_x = rand() % grid_size;
        *marker_y = rand() % grid_size;
    }

    draw_marker(*marker_x, *marker_y);
}

int marker_coinciding_with_block(int block_positions_x[], int block_positions_y[], int num_of_blocks, int marker_x,
                                 int marker_y) {
    for (int i = 0; i < num_of_blocks; i++) {
        if (block_positions_x[i] == marker_x && block_positions_y[i] == marker_y) return 1;
    }
    return 0;
}

void draw_marker(int x_coordinate, int y_coordinate) {
    setColour(gray);
    fillRect(start_x + (x_coordinate * width), start_y + (y_coordinate * width), width, width);
    // In the 2D-array, the coordinates of the marker must be set to 2
    grid[x_coordinate][y_coordinate] = 2;
}

void draw_blocks(int num_of_blocks, int *block_positions_x, int *block_positions_y) {
    setColour(red);
    int x_coordinate, y_coordinate;
    for (int i = 0; i < num_of_blocks; i++) {
        x_coordinate = block_positions_x[i];
        y_coordinate = block_positions_y[i];
        fillRect(start_x + (x_coordinate * width), start_y + (y_coordinate * width), width, width);
        // In the 2D-array, the coordinates of the block must be set to 1
        grid[x_coordinate][y_coordinate] = 1;
    }
}

void draw_robot(int x, int y, int direction) {
    int triangle_x_coordinates[3], triangle_y_coordinates[3];

    // The coordinates of the robot depend on the direction it is facing,
    // so we implement a switch statement
    switch (direction) {
        case north:
            robot_facing_north_x(x, triangle_x_coordinates);
            robot_facing_north_y(y, triangle_y_coordinates);
            break;
        case south:
            robot_facing_south_x(x, triangle_x_coordinates);
            robot_facing_south_y(y, triangle_y_coordinates);
            break;
        case west:
            robot_facing_west_x(x, triangle_x_coordinates);
            robot_facing_west_y(y, triangle_y_coordinates);
            break;
        case east:
            robot_facing_east_x(x, triangle_x_coordinates);
            robot_facing_east_y(y, triangle_y_coordinates);
            break;
    }
    setColour(green);
    fillPolygon(3, triangle_x_coordinates, triangle_y_coordinates);
}

// Functions to set the x and y coordinates of the robot depending on its direction

void robot_facing_north_x(int x, int *triangle_x_coordinates) {
    triangle_x_coordinates[0] = start_x + (x * width) + width / 2;
    triangle_x_coordinates[1] = start_x + (x * width);
    triangle_x_coordinates[2] = start_x + ((x + 1) * width);
}

void robot_facing_north_y(int y, int *triangle_y_coordinates) {
    triangle_y_coordinates[0] = start_y + (y * width);
    triangle_y_coordinates[1] = start_y + ((y + 1) * width);
    triangle_y_coordinates[2] = start_y + ((y + 1) * width);
}

void robot_facing_south_x(int x, int *triangle_x_coordinates) {
    triangle_x_coordinates[0] = start_x + (x * width);
    triangle_x_coordinates[1] = start_x + ((x + 1) * width);
    triangle_x_coordinates[2] = start_x + (x * width) + width / 2;
}

void robot_facing_south_y(int y, int *triangle_y_coordinates) {
    triangle_y_coordinates[0] = start_y + (y * width);
    triangle_y_coordinates[1] = start_y + ((y) * width);
    triangle_y_coordinates[2] = start_y + ((y + 1) * width);
}

void robot_facing_east_x(int x, int *triangle_x_coordinates) {
    triangle_x_coordinates[0] = start_x + (x * width);
    triangle_x_coordinates[1] = start_x + ((x + 1) * width);
    triangle_x_coordinates[2] = start_x + (x * width);
}

void robot_facing_east_y(int y, int *triangle_y_coordinates) {
    triangle_y_coordinates[0] = start_y + (y * width);
    triangle_y_coordinates[1] = start_y + (y * width) + width / 2;
    triangle_y_coordinates[2] = start_y + ((y + 1) * width);
}

void robot_facing_west_x(int x, int *triangle_x_coordinates) {
    triangle_x_coordinates[0] = start_x + ((x + 1) * width);
    triangle_x_coordinates[1] = start_x + (x * width);
    triangle_x_coordinates[2] = start_x + ((x + 1) * width);
}

void robot_facing_west_y(int y, int *triangle_y_coordinates) {
    triangle_y_coordinates[0] = start_y + (y * width);
    triangle_y_coordinates[1] = start_y + (y * width) + width / 2;
    triangle_y_coordinates[2] = start_y + ((y + 1) * width);
}

void left(struct robot *arobot) {
    clear();
    switch (arobot->direction) {
        case north:
            arobot->direction = west;
            break;
        case south:
            arobot->direction = east;
            break;
        case west:
            arobot->direction = south;
            break;
        case east:
            arobot->direction = north;
            break;
    }
    draw_robot(arobot->x_pos, arobot->y_pos, arobot->direction);
}


void right(struct robot *arobot) {
    clear();
    switch (arobot->direction) {
        case north:
            arobot->direction = east;
            break;
        case south:
            arobot->direction = west;
            break;
        case west:
            arobot->direction = north;
            break;
        case east:
            arobot->direction = south;
            break;
    }
    draw_robot(arobot->x_pos, arobot->y_pos, arobot->direction);
}

int not_stuck_at_wall(struct robot arobot) {
    switch (arobot.direction) {
        case north:
            if (arobot.y_pos <= 0) return 0;
            break;
        case south:
            if (grid_size - arobot.y_pos <= 1) return 0;
            break;
        case west:
            if (arobot.x_pos <= 0) return 0;
            break;
        case east:
            if (grid_size - arobot.x_pos <= 1) return 0;
            break;
    }
    return 1;
}


int not_stuck_at_block(struct robot arobot, int grid[][grid_size]) {
    switch (arobot.direction) {
        case north:
            if (grid[arobot.x_pos][arobot.y_pos - 1] == 1) return 0;
            break;
        case south:
            if (grid[arobot.x_pos][arobot.y_pos + 1] == 1) return 0;
            break;
        case west:
            if (grid[arobot.x_pos - 1][arobot.y_pos] == 1) return 0;
            break;
        case east:
            if (grid[arobot.x_pos + 1][arobot.y_pos] == 1) return 0;
            break;
    }
    return 1;
}

int canMoveForward(struct robot arobot) {
    return not_stuck_at_block(arobot, grid) && not_stuck_at_wall(arobot);
}

void forward(struct robot *arobot) {
    if (canMoveForward(*arobot)) {
        clear();
        switch (arobot->direction) {
            case north:
                arobot->y_pos--;
                break;
            case south:
                arobot->y_pos++;
                break;
            case west:
                arobot->x_pos--;
                break;
            case east:
                arobot->x_pos++;
                break;
        }
        draw_robot(arobot->x_pos, arobot->y_pos, arobot->direction);
    } else {
        // If the robot cannot move forward, we turn it either left or right
        if (rand() % 2) right(arobot);
        else left(arobot);
    }
}

int atMarker(struct robot arobot, int marker_x, int marker_y) {
    return (arobot.x_pos == marker_x && arobot.y_pos == marker_y);
}
