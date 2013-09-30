#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <string>
#include "settings.h"

#ifndef WINDOW_H
#define WINDOW_H

// All possible colours that can be drawn
enum Color {White, Black, Red, Green, Blue, Orange, Yellow, Magenta, DarkOrange, Brown, Grey, DarkGrey, DarkBlue};

enum Key { Left = XK_Left, Right = XK_Right, Up = XK_j, Down = XK_Down, Pause = XK_space, Quit = XK_q };

// Class for the Game Window
class GameWindow {

private:
	Display*  	display; // Display
	Window   	window;  // The window, only drawn on from buffer
	Pixmap 		buffer;  // Double buffer
	GC			gc;		 // Graphics Context
	int 		screen;	 // Screen ID

	std::vector<unsigned long> colors; // color codes

	// Position to draw the screen
	int x_offset;
	int y_offset;

	// initialize components
	void initWindow();
	void initGC();
	void initColors();
	
	// Handle the resize event
	void handleResize(XEvent&);

public:
	
	// Constructor
	GameWindow();

	void close();

	// Repaint the window from the buffer
	void repaint();

	// Set the color of what will be drawn, bg == true if setting the background color
	void setColor(Color c, bool bg=false);
	void setThickness(int t);
	
	// Drawing lines and shapes
	void drawLine(int x1, int y1, int x2, int y2);
	void drawRect(int x, int y, size_t width, size_t hieght, bool fill=false);
	void drawCircle(int x, int y, size_t radius, bool fill=false);
	void drawText(int x, int y, const std::string& text);	
	void drawTrig(int x, int y, int width, int height, bool fill=false);
	void drawOval(int x, int y, int width, int height, bool fill=false);

	// Collect events such as key presses and resizes 
	int getKeyEvent(Key&);
};



/* An abstract class for a drawable object. Any object that appears on the screen
 should inherit from this class. The layer that the object is being drawn on must
 be given upon construction */
class Picture {

public:	
	virtual void nextFrame() { };
	// Pure virtual function to be defined for all separate drawable object
	virtual void paint(GameWindow& GW) = 0;
};


// The background drawable image
class Background : public Picture {

public:
	void paint(GameWindow& GW);
};






#endif
