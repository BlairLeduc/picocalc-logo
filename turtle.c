//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#include <math.h>

#include "turtle.h"

// Turtle state
float turtle_x = TURTLE_HOME_X;                  // Current x position for graphics
float turtle_y = TURTLE_HOME_Y;                  // Current y position for graphics
float turtle_angle = TURTLE_DEFAULT_ANGLE;       // Current angle for graphics
uint16_t turtle_colour = TURTLE_DEFAULT_COLOUR;    // Turtle color for graphics
static bool turtle_pen_down = TURTLE_DEFAULT_PEN_DOWN;  // Pen state for graphics
static bool turtle_visible = TURTLE_DEFAULT_VISIBILITY; // Turtle visibility state for graphics


//
//  Turtle graphics functions
//  

// Clear the graphics buffer and reset the turtle to the home position
void turtle_clearscreen(void)
{
    // Clear the graphics buffer
    screen_gfx_clear();

    // Reset the turtle to the home position
    turtle_x = TURTLE_HOME_X;
    turtle_y = TURTLE_HOME_Y;
    turtle_angle = TURTLE_DEFAULT_ANGLE;

    // Draw the turtle at the home position
    turtle_draw();

    // Update the graphics display
    screen_gfx_update();
}

//  Draw the turtle at the current position
void turtle_draw()
{
    // Convert the angle to radians
    float radians = turtle_angle * (M_PI / 180.0f);

    // Calculate the vertices of the turtle triangle
    float half_base = 4.0f; // Half the base width of the turtle triangle
    float height = 12.0f;   // Height of the turtle triangle

    // Calculate the three vertices of the triangle
    float x1 = turtle_x + half_base * cosf(radians);
    float y1 = turtle_y + half_base * sinf(radians);
    float x2 = turtle_x - half_base * cosf(radians);
    float y2 = turtle_y - half_base * sinf(radians);
    float x3 = turtle_x + height * sinf(radians);
    float y3 = turtle_y - height * cosf(radians);

    // Draw the triangle using lines
    screen_gfx_line(x1, y1, x2, y2, turtle_colour, true);
    screen_gfx_line(x2, y2, x3, y3, turtle_colour, true);
    screen_gfx_line(x3, y3, x1, y1, turtle_colour, true);
}

// Move the turtle forward or backward by the specified distance
void turtle_move(float distance)
{
    float x = turtle_x;
    float y = turtle_y;

    turtle_draw();

    // Move the turtle forward by the specified distance
    turtle_x += distance * sinf(turtle_angle * (M_PI / 180.0f));
    turtle_y -= distance * cosf(turtle_angle * (M_PI / 180.0f));

    // Draw the turtle at the new position
    
    if (turtle_pen_down)
    {
        // Draw a line from the old position to the new position
        screen_gfx_line(x, y, turtle_x, turtle_y, turtle_colour, false);
    }

    turtle_draw();
    
    // Ensure the turtle stays within bounds
    turtle_x = fmodf(turtle_x + SCREEN_WIDTH, SCREEN_WIDTH);
    turtle_y = fmodf(turtle_y + SCREEN_HEIGHT, SCREEN_HEIGHT);

    screen_gfx_update();
}

// Reset the turtle to the home position
void turtle_home(void)
{
    // Draw the turtle at the home position
    turtle_draw();

    // Reset the turtle to the home position
    turtle_x = TURTLE_HOME_X;
    turtle_y = TURTLE_HOME_Y;
    turtle_angle = TURTLE_DEFAULT_ANGLE;

    // Draw the turtle at the home position
    turtle_draw();
    
    screen_gfx_update();
}

// Set the turtle position to the specified coordinates
void turtle_set_position(float x, float y)
{
    // Draw the current turtle position before moving
    turtle_draw();

    // Set the new position
    turtle_x = fmodf(x + SCREEN_WIDTH, SCREEN_WIDTH);
    turtle_y = fmodf(y + SCREEN_HEIGHT, SCREEN_HEIGHT);

    // Draw the turtle at the new position
    turtle_draw();
    screen_gfx_update();
}

// Get the current turtle position
void turtle_get_position(float *x, float *y)
{
    if (x)
    {
        *x = turtle_x;
    }
    if (y)
    {
        *y = turtle_y;
    }
}

// Set the turtle angle to the specified value
void turtle_set_angle(float angle)
{
    turtle_draw();
    turtle_angle = fmodf(angle, 360.0f); // Normalize the angle
    turtle_draw();
}

// Get the current turtle angle
float turtle_get_angle(void)
{
    return turtle_angle; // Return the current angle
}

// Set the turtle color to the specified value
void turtle_set_colour(uint16_t colour)
{
    // Draw the current turtle position before changing color
    turtle_draw(turtle_x, turtle_y, turtle_angle);

    // Set the new turtle color
    turtle_colour = colour;

    // Draw the turtle at the current position with the new color
    turtle_draw(turtle_x, turtle_y, turtle_angle);
    screen_gfx_update();
}

// Get the current turtle color
uint16_t turtle_get_colour(void)
{
    return turtle_colour; // Return the current turtle color
}

// Set the pen state (down or up)
void turtle_set_pen_down(bool down)
{
    turtle_pen_down = down; // Set the pen state
}

// Get the current pen state (down or up)
bool turtle_get_pen_down(void)
{
    return turtle_pen_down; // Return the current pen state
}

// Set the turtle visibility (visible or hidden)
void turtle_set_visibility(bool visible)
{
    if (turtle_visible == visible)
    {
        return; // No change in visibility
    }

    turtle_draw(); // XOR the current turtle to draw/erase it
    turtle_visible = visible;
}

// Draw or erase the turtle based on visibility
bool turtle_get_visibility(void)
{
    return turtle_visible;
}

