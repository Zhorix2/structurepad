#include <curses.h>
#include <iostream>

int main() {
    // Initialize curses
    initscr();
    
    // Enable color if available
    if (has_colors()) {
        start_color();
    }
    
    // Clear screen
    clear();
    
    // Print message
    printw("Welcome to StructurePad!\n");
    printw("PDCurses-based application\n");
    printw("Press any key to exit...\n");
    
    // Refresh the display
    refresh();
    
    // Wait for user input
    getch();
    
    // Clean up and exit
    endwin();
    
    return 0;
}
