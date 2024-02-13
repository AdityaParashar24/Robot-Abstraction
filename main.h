// Global variables
const int width = 60, start_x = 60, start_y = 40, grid_size = 10;
const int north = 1, east = 2, south = 3, west = 4;
const int end_x = start_x + (width * grid_size), end_y = start_y + (width * grid_size);
int grid[grid_size][grid_size] = {0};

// Structure
struct robot {
    int x_pos;
    int y_pos;
    int direction;
};

// Functions to draw background
void draw_columns(int end_x, int end_y);

void draw_rows(int end_x, int end_y);

void draw_marker(int x_coordinate, int y_coordinate);

void draw_blocks(int num_of_blocks, int *block_positions_x, int *block_positions_y);

void set_background_colour(int window_size_x, int window_size_y);

int marker_coinciding_with_block(int block_positions_x[], int block_positions_y[],
                                 int num_of_blocks, int marker_x, int marker_y);

void setting_background(int *marker_x, int *marker_y);

// Functions for foreground animations
int check_if_input_valid(struct robot arobot);

void init_robot(struct robot *arobot, int robot_initial_x, int robot_initial_y, char *robot_initial_direction_str);

void draw_robot(int x, int y, int direction);

void robot_facing_north_x(int x, int *triangle_x_coordinates);

void robot_facing_north_y(int y, int *triangle_y_coordinates);

void robot_facing_south_x(int x, int *triangle_x_coordinates);

void robot_facing_south_y(int y, int *triangle_y_coordinates);

void robot_facing_east_x(int x, int *triangle_x_coordinates);

void robot_facing_east_y(int y, int *triangle_y_coordinates);

void robot_facing_west_x(int x, int *triangle_x_coordinates);

void robot_facing_west_y(int y, int *triangle_y_coordinates);

void left(struct robot *arobot);

void right(struct robot *arobot);

int not_stuck_at_wall(struct robot arobot);

int not_stuck_at_block(struct robot arobot, int grid[][grid_size]);

int canMoveForward(struct robot arobot);

void forward(struct robot *arobot);

int atMarker(struct robot arobot, int marker_x, int marker_y);