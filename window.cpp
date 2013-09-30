#include "window.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>

// Output an error message and quit
void error(const std::string& msg) {
	std::cerr << msg << std::endl;
	exit(0);
}

// Initialize the window
void GameWindow::initWindow() { 

	unsigned long white = XWhitePixel(display, screen);
    unsigned long black = XBlackPixel(display, screen);

	// Make Size hints
	XSizeHints hints;
    hints.x = DISPLAY_X;
    hints.y = DISPLAY_Y;
    hints.width = DISPLAY_WIDTH;
    hints.height = DISPLAY_HEIGHT;
    hints.flags = PPosition | PSize;

	// Create a window
	window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        DISPLAY_X, DISPLAY_Y,
        DISPLAY_WIDTH, DISPLAY_HEIGHT,
        DISPLAY_BORDER, black, black);
	
	XMapWindow(display,window);
	XFlush(display);
	usleep(4000); 	// <- Very important
}


// Initialize Graphics Context
void GameWindow::initGC() {
 	
	gc = XCreateGC(display, window, 0, 0);
	unsigned int line_width = 3;		

	Font font;
	font = XLoadFont(display, "*x24");
	XSetFont(display,gc,font);
	XSetForeground(display, gc, XBlackPixel(display,screen));
	XSetBackground(display, gc, XWhitePixel(display,screen));
  
	XSetLineAttributes(display, gc, line_width, LineSolid, CapRound, JoinRound);
	XSetFillStyle(display, gc, FillOpaqueStippled);
}


// Initialize colours
void GameWindow::initColors() {
	
	Colormap cmap; 
	cmap = DefaultColormap(display, screen);
	XColor xc;
	bool good_color;

	// All color names as strings
	std::string cnames[NUM_COLORS] = COLOR_NAMES; 
	
	// Parse each color name and allocate for it
	for (int i = 0; i < NUM_COLORS; i++) {
		good_color = XParseColor(display, cmap, cnames[i].c_str(), &xc);
		good_color |= XAllocColor(display, cmap, &xc); 
		if (good_color) colors[i] = xc.pixel;
		else error("Could not parse color");
	}
}


// Initialize a GameWindow
GameWindow::GameWindow(): x_offset(0), y_offset(0), colors(NUM_COLORS) {

	// Grab the display and check if it can open
	display = XOpenDisplay("");
	if (! display) error("Cannot open display.");

	// Initialize the screen, colors, window, and graphics context
	screen = DefaultScreen(display);
	initColors();
	initWindow();
	initGC();

	// Initialize the buffer pixmap
	int depth = DefaultDepth(display, screen);
	buffer = XCreatePixmap (display, window, DISPLAY_WIDTH, DISPLAY_HEIGHT, depth);

	XAutoRepeatOff(display);

	XSelectInput(display,window, 
		KeyPressMask | 
		KeyReleaseMask |
		StructureNotifyMask);
}

void GameWindow::close() {
	XAutoRepeatOn(display);
	XFlush(display);
}

void GameWindow::handleResize(XEvent& event) {

	XConfigureEvent resize = event.xconfigure;
	x_offset = (resize.width - DISPLAY_WIDTH) / 2;
	y_offset = (resize.height - DISPLAY_HEIGHT) / 2;
}

// Look up the key value of the key-event
Key lookUpKey(XKeyEvent& event) {
	KeySym keysym;
	XComposeStatus compose;
	XLookupString(&event, NULL, 0, &keysym, &compose);
	return Key(keysym);
}

// Predicate function for use in XCheckEvent for any valid event
int valid_event(Display* display, XEvent* event, XPointer not_used) {
	int t = event->type;
	return (t == KeyPress || t == ConfigureNotify || t == KeyRelease); 
}

// Get the lastest key event
int GameWindow::getKeyEvent(Key& result) {
	XEvent event;
	
	// Check if an event has happened
	if (XCheckIfEvent(display,&event,&valid_event,NULL)) {
		// If Resize event:
		if (event.type == ConfigureNotify) {
			handleResize(event);
			return 0;
		// if Keybutton event:
		} else {
			result = lookUpKey(event.xkey);
			XSync(display,true);
			return event.type;
		}
	}
}


// Change the color for drawing
void GameWindow::setColor(Color c, bool bg) {

	if (bg) 
		XSetBackground(display, gc, colors[c]);
	else	
		XSetForeground(display, gc, colors[c]);
}


// Repaint the screen from the buffer
void GameWindow::repaint() {

	// If window is too small, put message
	if (x_offset < 0 || y_offset < 0) {
		setColor(Black);
		drawRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT,true);
		setColor(White, false); // FG
		setColor(Black, true);	// BG
		int width = x_offset*2 + DISPLAY_WIDTH;
		int height = y_offset*2 + DISPLAY_HEIGHT;
		drawText(width/2 - MSG_LENGTH,height/2,"Window too Small!");	
	}
	
	// Otherwise repaint as normal
	XCopyArea(display, buffer, window, gc, 0,0, 
		DISPLAY_WIDTH, DISPLAY_HEIGHT, 
		x_offset, y_offset);

	XFlush(display);
}

// Draw a line from (x1,y1) -> (x2,y2)
void GameWindow::drawLine(int x1, int y1, int x2, int y2) {
	XDrawLine(display,buffer,gc, x1, y1, x2, y2);
}

// Draw a rectangle
void GameWindow::drawRect(int x, int y, size_t width, size_t height, bool fill) {

	if (fill)
		XFillRectangle(display,buffer,gc, x, y, width, height);
	else 
		XDrawRectangle(display,buffer,gc, x, y, width, height);
}

// Draw a circle
void GameWindow::drawCircle(int x, int y, size_t radius, bool fill) {
	size_t diameter = 2 * radius;
	if (fill)
	    XFillArc(display,buffer,gc, x - radius, y - radius, diameter, diameter, 0, CIRCLE_ANGLE);	
	else
		XDrawArc(display,buffer,gc, x - radius, y - radius, diameter, diameter, 0, CIRCLE_ANGLE);
}

void GameWindow::drawOval(int x, int y, int width, int height, bool fill) {
	if (fill)
		XFillArc(display,buffer,gc,x,y,width,height,0,CIRCLE_ANGLE);
	else
		XDrawArc(display,buffer,gc,x,y,width,height,0,CIRCLE_ANGLE);
}

// Draw some text onto the screen
void GameWindow::drawText(int x, int y, const std::string& msg) {

	XDrawImageString(display,buffer,gc, x, y, msg.c_str(), msg.length()); 
}

void GameWindow::drawTrig(int x, int y, int width ,int height, bool fill) {
	
	XPoint vert[3];
	vert[0].x = x;
	vert[0].y = y + height;
	vert[1].x = x + width/2;
	vert[1].y = y;
	vert[2].x = x + width;
	vert[2].y = y + height;

	if (fill)
		XFillPolygon(display,buffer,gc,&vert[0],3,Convex,CoordModeOrigin);
	else
		XDrawLines(display,buffer,gc,&vert[0],3,CoordModeOrigin);
}

#include "level.h"

// Draw White background
void Background::paint(GameWindow& GW) {
	GW.setColor(White);
	GW.drawRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT,true);
	GW.setColor(Blue);
	int y_off = Level::y_offset * PAPER_SCALE;	

	for (int i = 90; i < DISPLAY_WIDTH; i += 30) {
	//	GW.drawLine(i,0,i,DISPLAY_HEIGHT);	
		GW.drawLine(0,i-y_off,DISPLAY_WIDTH,i-y_off);
	}
	GW.setColor(Magenta);
	int x_off = Level::x_offset * PAPER_SCALE;
	GW.drawLine(150-x_off,0,150-x_off,DISPLAY_HEIGHT);
}


