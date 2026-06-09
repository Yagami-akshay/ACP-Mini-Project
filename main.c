#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#define ROWS 20
#define COLS 60
#define MAX_SHAPES 100
#define MIN_ROWS 28
#define MIN_COLS 85
// Types of shapes supported
typedef enum {
    SHAPE_LINE,
Shape shapes[MAX_SHAPES];
int num_shapes = 0;
char canvas[ROWS][COLS];
char (*active_canvas)[COLS] = canvas;
char highlight_mask[ROWS][COLS];
int show_highlight = 0;
int highlight_shape_idx = -1;
WINDOW *title_win = NULL;
WINDOW *canvas_win = NULL;
WINDOW *sidebar_win = NULL;
WINDOW *status_win = NULL;
// Function Prototypes
void clear_canvas();
void plot_point(int x, int y);
void draw_line(int x0, int y0, int x1, int y1);
void draw_rectangle(int x, int y, int width, int height);
void draw_circle(int cx, int cy, int r);
void plot_circle_points(int cx, int cy, int x, int y);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void draw_shape(Shape s);
void render_canvas();
void render_preview_canvas(Shape temp_shape);
void render_canvas_with_highlight(int highlight_idx);
void display_canvas();
void list_shapes();
void delete_shape_ui();
int read_int(const char *prompt);
int main() {
void run_cli_mode();
void run_tui_mode();
void init_tui();
void check_term_size();
void create_windows();
void destroy_windows();
void display_canvas_tui();
void display_sidebar_menu(int selected_index);
void display_sidebar_shapes(int highlighted_idx);
int pick_coordinates(int *out_x, int *out_y, int has_preview, Shape temp_shape);
int main(int argc, char **argv) {
    int cli_mode = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--cli") == 0) {
            cli_mode = 1;
        }
    }
    
    if (cli_mode) {
        run_cli_mode();
    } else {
        run_tui_mode();
    }
    return 0;
}
// Wrap original console menu loop
void run_cli_mode() {
    int choice;
    printf("==========================================\n");
    printf(" Welcome to the 2D character Graphics Editor\n");
    printf(" Welcome to the 2D character Graphics Editor (CLI Mode)\n");
    printf("==========================================\n");
    printf("Canvas size: %d columns (X: 0 to %d) x %d rows (Y: 0 to %d)\n", COLS, COLS - 1, ROWS, ROWS - 1);
    printf("Coordinates out of bounds will be clipped automatically.\n");
                break;
            case 9:
                printf("Exiting... Goodbye!\n");
                return 0;
                return;
            default:
                printf("Invalid choice. Please enter a number between 1 and 9.\n");
        }
    }
    return 0;
}
// Clear the canvas to the background character '_'
void clear_canvas() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            canvas[r][c] = '_';
            active_canvas[r][c] = '_';
        }
    }
}
// Plot a point if it falls within the canvas boundary
void plot_point(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        // Map Cartesian (0,0 is bottom-left) to 2D array row indexes
        int r = ROWS - 1 - y;
        canvas[r][x] = '*';
        active_canvas[r][x] = '*';
    }
}
// Rectangle Drawing Function
void draw_rectangle(int x, int y, int width, int height) {
    if (width <= 0 || height <= 0) return;
    draw_line(x, y, x + width - 1, y);                               // Bottom edge
    draw_line(x, y + height - 1, x + width - 1, y + height - 1);     // Top edge
    draw_line(x, y, x, y + height - 1);                             // Left edge
    draw_line(x + width - 1, y, x + width - 1, y + height - 1);     // Right edge
    draw_line(x, y, x + width - 1, y);
    draw_line(x, y + height - 1, x + width - 1, y + height - 1);
    draw_line(x, y, x, y + height - 1);
    draw_line(x + width - 1, y, x + width - 1, y + height - 1);
}
// Midpoint Circle Algorithm
    }
}
// Helper to plot 8-way symmetric points for circle
void plot_circle_points(int cx, int cy, int x, int y) {
    plot_point(cx + x, cy + y);
    plot_point(cx - x, cy + y);
    plot_point(cx + x, cy - y);
    plot_point(cx - x, cy - y);
    plot_point(cx + y, cy + x);
    plot_point(cx - y, cy + x);
    plot_point(cx + y, cy - x);
    plot_point(cx - y, cy - x);
}
// Triangle Drawing Function
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}
// Render all active shapes on canvas
void draw_shape(Shape s) {
    switch (s.type) {
        case SHAPE_LINE:
            draw_line(s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
            break;
        case SHAPE_RECTANGLE:
            draw_rectangle(s.data.rect.x, s.data.rect.y, s.data.rect.width, s.data.rect.height);
            break;
        case SHAPE_CIRCLE:
            draw_circle(s.data.circle.cx, s.data.circle.cy, s.data.circle.r);
            break;
        case SHAPE_TRIANGLE:
            draw_triangle(s.data.triangle.x1, s.data.triangle.y1,
                          s.data.triangle.x2, s.data.triangle.y2,
                          s.data.triangle.x3, s.data.triangle.y3);
            break;
    }
}
void render_canvas() {
    active_canvas = canvas;
    clear_canvas();
    for (int i = 0; i < num_shapes; i++) {
        Shape s = shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                draw_line(s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(s.data.rect.x, s.data.rect.y, s.data.rect.width, s.data.rect.height);
                break;
            case SHAPE_CIRCLE:
                draw_circle(s.data.circle.cx, s.data.circle.cy, s.data.circle.r);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(s.data.triangle.x1, s.data.triangle.y1,
                              s.data.triangle.x2, s.data.triangle.y2,
                              s.data.triangle.x3, s.data.triangle.y3);
                break;
        draw_shape(shapes[i]);
    }
}
void render_preview_canvas(Shape temp_shape) {
    active_canvas = canvas;
    clear_canvas();
    for (int i = 0; i < num_shapes; i++) {
        draw_shape(shapes[i]);
    }
    draw_shape(temp_shape);
}
void render_canvas_with_highlight(int highlight_idx) {
    active_canvas = canvas;
    clear_canvas();
    for (int i = 0; i < num_shapes; i++) {
        draw_shape(shapes[i]);
    }
    
    active_canvas = highlight_mask;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            highlight_mask[r][c] = '_';
        }
    }
    draw_shape(shapes[highlight_idx]);
    
    active_canvas = canvas;
}
// Display the canvas in the terminal with boundary lines and coordinate labels
void display_canvas() {
    printf("\nCanvas State:\n");
    // Top boundary line
    printf("   +");
    for (int col = 0; col < COLS; col++) printf("-");
    printf("+\n");
    // Display rows from top to bottom
    for (int r = 0; r < ROWS; r++) {
        // Y-axis label (Cartesian y corresponding to this row index)
        int y = ROWS - 1 - r;
        printf("%2d |", y);
        for (int c = 0; c < COLS; c++) {
            printf("%c", canvas[r][c]);
        }
        printf("|\n");
    }
    // Bottom boundary line
    printf("   +");
    for (int col = 0; col < COLS; col++) printf("-");
    printf("+\n");
    // X-axis tens place label
    printf("    ");
    for (int col = 0; col < COLS; col++) {
        if (col % 10 == 0) {
            printf("%d", col / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");
    // X-axis ones place label
    printf("    ");
    for (int col = 0; col < COLS; col++) {
        printf("%d", col % 10);
    }
    printf("\n");
}
// List all currently active shapes
void list_shapes() {
    if (num_shapes == 0) {
        printf("\nNo objects in the picture.\n");
    printf("-----------------------\n");
}
// User interface workflow to delete a shape
void delete_shape_ui() {
    if (num_shapes == 0) {
        printf("\nNo objects to delete.\n");
        return;
    }
    list_shapes();
    int idx = read_int("Enter the index of the object to delete: ");
    if (idx < 0 || idx >= num_shapes) {
        printf("Invalid index! No object deleted.\n");
        return;
    }
    
    // Shift remaining shapes
    for (int i = idx; i < num_shapes - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    num_shapes--;
    printf("Object at index %d deleted successfully.\n", idx);
}
// Safe integer reader that prevents infinite loops on invalid input
int read_int(const char *prompt) {
    int val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Remove newline
            buffer[strcspn(buffer, "\n")] = '\0';
            
            // Try parsing integer
            char *endptr;
            val = (int)strtol(buffer, &endptr, 10);
            if (endptr != buffer && *endptr == '\0') {
                return val;
            }
        }
        printf("Invalid input. Please enter a valid integer.\n");
    }
}
// -------------------------------------------------------------
// NCURSES TUI IMPLEMENTATION
// -------------------------------------------------------------
void init_tui() {
    setenv("ESCDELAY", "25", 1);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_WHITE, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_RED, -1);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_WHITE, COLOR_RED);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
}
void check_term_size() {
    int r, c;
    getmaxyx(stdscr, r, c);
    while (r < MIN_ROWS || c < MIN_COLS) {
        clear();
        mvprintw(0, 0, "Terminal window is too small!");
        mvprintw(1, 0, "Current size: %d columns x %d rows", c, r);
        mvprintw(2, 0, "Required size: %d columns x %d rows", MIN_COLS, MIN_ROWS);
        mvprintw(4, 0, "Please enlarge your terminal window...");
        mvprintw(5, 0, "Or press 'Q' to quit.");
        refresh();
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            endwin();
            exit(0);
        }
        getmaxyx(stdscr, r, c);
    }
}
void create_windows() {
    destroy_windows();
    title_win = newwin(1, 85, 0, 0);
    canvas_win = newwin(24, 65, 1, 0);
    sidebar_win = newwin(24, 20, 1, 65);
    status_win = newwin(3, 85, 25, 0);
    
    keypad(canvas_win, TRUE);
    keypad(sidebar_win, TRUE);
    keypad(status_win, TRUE);
}
void destroy_windows() {
    if (title_win) { delwin(title_win); title_win = NULL; }
    if (canvas_win) { delwin(canvas_win); canvas_win = NULL; }
    if (sidebar_win) { delwin(sidebar_win); sidebar_win = NULL; }
    if (status_win) { delwin(status_win); status_win = NULL; }
}
void display_canvas_tui() {
    wclear(canvas_win);
    
    wattron(canvas_win, COLOR_PAIR(1));
    mvwprintw(canvas_win, 0, 3, "+");
    for (int col = 0; col < COLS; col++) wprintw(canvas_win, "-");
    wprintw(canvas_win, "+");
    wattroff(canvas_win, COLOR_PAIR(1));
    
    for (int r = 0; r < ROWS; r++) {
        int y = ROWS - 1 - r;
        
        wattron(canvas_win, COLOR_PAIR(1));
        mvwprintw(canvas_win, r + 1, 0, "%2d |", y);
        wattroff(canvas_win, COLOR_PAIR(1));
        
        for (int c = 0; c < COLS; c++) {
            if (show_highlight && highlight_mask[r][c] == '*') {
                wattron(canvas_win, COLOR_PAIR(7) | A_BOLD | A_BLINK);
                waddch(canvas_win, '*');
                wattroff(canvas_win, COLOR_PAIR(7) | A_BOLD | A_BLINK);
            } else if (canvas[r][c] == '*') {
                wattron(canvas_win, COLOR_PAIR(4) | A_BOLD);
                waddch(canvas_win, '*');
                wattroff(canvas_win, COLOR_PAIR(4) | A_BOLD);
            } else {
                wattron(canvas_win, COLOR_PAIR(2));
                waddch(canvas_win, '_');
                wattroff(canvas_win, COLOR_PAIR(2));
            }
        }
        
        wattron(canvas_win, COLOR_PAIR(1));
        wprintw(canvas_win, "|");
        wattroff(canvas_win, COLOR_PAIR(1));
    }
    
    wattron(canvas_win, COLOR_PAIR(1));
    mvwprintw(canvas_win, ROWS + 1, 3, "+");
    for (int col = 0; col < COLS; col++) wprintw(canvas_win, "-");
    wprintw(canvas_win, "+");
    
    mvwprintw(canvas_win, ROWS + 2, 4, "");
    for (int col = 0; col < COLS; col++) {
        if (col % 10 == 0) wprintw(canvas_win, "%d", col / 10);
        else wprintw(canvas_win, " ");
    }
    mvwprintw(canvas_win, ROWS + 3, 4, "");
    for (int col = 0; col < COLS; col++) {
        wprintw(canvas_win, "%d", col % 10);
    }
    wattroff(canvas_win, COLOR_PAIR(1));
    wrefresh(canvas_win);
}
void display_sidebar_menu(int selected_index) {
    wclear(sidebar_win);
    box(sidebar_win, 0, 0);
    
    wattron(sidebar_win, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(sidebar_win, 1, 2, " 2D EDITOR TUI ");
    wattroff(sidebar_win, COLOR_PAIR(1) | A_BOLD);
    
    mvwprintw(sidebar_win, 2, 2, "Objects: %d/%d", num_shapes, MAX_SHAPES);
    
    mvwhline(sidebar_win, 3, 1, ACS_HLINE, 18);
    
    const char *menu_options[] = {
        " Add Line     ",
        " Add Rect     ",
        " Add Circle   ",
        " Add Triangle ",
        " Delete Shape ",
        " Clear Canvas ",
        " Quit         "
    };
    int num_options = 7;
    
    for (int i = 0; i < num_options; i++) {
        if (i == selected_index) {
            wattron(sidebar_win, COLOR_PAIR(6));
            mvwprintw(sidebar_win, 5 + i * 2, 2, "%s", menu_options[i]);
            wattroff(sidebar_win, COLOR_PAIR(6));
        } else {
            mvwprintw(sidebar_win, 5 + i * 2, 2, "%s", menu_options[i]);
        }
    }
    
    mvwhline(sidebar_win, 19, 1, ACS_HLINE, 18);
    mvwprintw(sidebar_win, 20, 2, "Keys: L,R,C,T");
    mvwprintw(sidebar_win, 21, 2, "      D, X, Q");
    
    wrefresh(sidebar_win);
}
void display_sidebar_shapes(int highlighted_idx) {
    wclear(sidebar_win);
    box(sidebar_win, 0, 0);
    
    wattron(sidebar_win, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(sidebar_win, 1, 2, "SELECT SHAPE");
    wattroff(sidebar_win, COLOR_PAIR(1) | A_BOLD);
    
    mvwhline(sidebar_win, 2, 1, ACS_HLINE, 18);
    
    if (num_shapes == 0) {
        mvwprintw(sidebar_win, 4, 2, "No objects.");
        mvwprintw(sidebar_win, 6, 2, "Press ESC");
        wrefresh(sidebar_win);
        return;
    }
    
    int start_idx = 0;
    if (highlighted_idx >= 15) {
        start_idx = highlighted_idx - 14;
    }
    
    for (int i = start_idx; i < num_shapes && i < start_idx + 15; i++) {
        Shape s = shapes[i];
        char shape_name[25];
        switch (s.type) {
            case SHAPE_LINE:
                snprintf(shape_name, sizeof(shape_name), "[%d] Line", i);
                break;
            case SHAPE_RECTANGLE:
                snprintf(shape_name, sizeof(shape_name), "[%d] Rect", i);
                break;
            case SHAPE_CIRCLE:
                snprintf(shape_name, sizeof(shape_name), "[%d] Circle", i);
                break;
            case SHAPE_TRIANGLE:
                snprintf(shape_name, sizeof(shape_name), "[%d] Tri", i);
                break;
        }
        
        if (i == highlighted_idx) {
            wattron(sidebar_win, COLOR_PAIR(7));
            mvwprintw(sidebar_win, 3 + (i - start_idx), 2, "%-16s", shape_name);
            wattroff(sidebar_win, COLOR_PAIR(7));
        } else {
            mvwprintw(sidebar_win, 3 + (i - start_idx), 2, "%-16s", shape_name);
        }
    }
    
    mvwhline(sidebar_win, 19, 1, ACS_HLINE, 18);
    mvwprintw(sidebar_win, 20, 2, "Enter: Delete");
    mvwprintw(sidebar_win, 21, 2, "ESC: Cancel");
    
    wrefresh(sidebar_win);
}
int pick_coordinates(int *out_x, int *out_y, int has_preview, Shape temp_shape) {
    int cur_x = COLS / 2;
    int cur_y = ROWS / 2;
    
    if (has_preview) {
        if (temp_shape.type == SHAPE_LINE) {
            cur_x = temp_shape.data.line.x2;
            cur_y = temp_shape.data.line.y2;
        } else if (temp_shape.type == SHAPE_RECTANGLE) {
            cur_x = temp_shape.data.rect.x + temp_shape.data.rect.width - 1;
            cur_y = temp_shape.data.rect.y + temp_shape.data.rect.height - 1;
        } else if (temp_shape.type == SHAPE_CIRCLE) {
            cur_x = temp_shape.data.circle.cx + temp_shape.data.circle.r;
            cur_y = temp_shape.data.circle.cy;
        } else if (temp_shape.type == SHAPE_TRIANGLE) {
            cur_x = temp_shape.data.triangle.x3;
            cur_y = temp_shape.data.triangle.y3;
        }
    }
    while (1) {
        if (has_preview) {
            switch (temp_shape.type) {
                case SHAPE_LINE:
                    temp_shape.data.line.x2 = cur_x;
                    temp_shape.data.line.y2 = cur_y;
                    break;
                case SHAPE_RECTANGLE: {
                    int x1 = temp_shape.data.rect.x;
                    int y1 = temp_shape.data.rect.y;
                    temp_shape.data.rect.x = x1 < cur_x ? x1 : cur_x;
                    temp_shape.data.rect.y = y1 < cur_y ? y1 : cur_y;
                    temp_shape.data.rect.width = abs(cur_x - x1) + 1;
                    temp_shape.data.rect.height = abs(cur_y - y1) + 1;
                    break;
                }
                case SHAPE_CIRCLE: {
                    int cx = temp_shape.data.circle.cx;
                    int cy = temp_shape.data.circle.cy;
                    double dist = sqrt((cur_x - cx)*(cur_x - cx) + (cur_y - cy)*(cur_y - cy));
                    temp_shape.data.circle.r = (int)(dist + 0.5);
                    break;
                }
                case SHAPE_TRIANGLE:
                    temp_shape.data.triangle.x3 = cur_x;
                    temp_shape.data.triangle.y3 = cur_y;
                    break;
            }
            render_preview_canvas(temp_shape);
        } else {
            render_canvas();
        }
        
        display_canvas_tui();
        
        int win_row = ROWS - cur_y;
        int win_col = cur_x + 4;
        
        wattron(canvas_win, COLOR_PAIR(8) | A_BLINK);
        char current_char = canvas[ROWS - 1 - cur_y][cur_x];
        mvwaddch(canvas_win, win_row, win_col, current_char);
        wattroff(canvas_win, COLOR_PAIR(8) | A_BLINK);
        wrefresh(canvas_win);
        
        wclear(status_win);
        box(status_win, 0, 0);
        mvwprintw(status_win, 1, 2, "Cursor: (%2d, %2d) | Arrows: Move | Enter: Confirm | ESC: Cancel", cur_x, cur_y);
        wrefresh(status_win);
        
        int ch = getch();
        if (ch == KEY_RESIZE) {
            check_term_size();
            create_windows();
            continue;
        }
        
        switch (ch) {
            case KEY_UP:
                if (cur_y < ROWS - 1) cur_y++;
                break;
            case KEY_DOWN:
                if (cur_y > 0) cur_y--;
                break;
            case KEY_LEFT:
                if (cur_x > 0) cur_x--;
                break;
            case KEY_RIGHT:
                if (cur_x < COLS - 1) cur_x++;
                break;
            case 27:
            case 'q':
            case 'Q':
                return 0;
            case '\n':
            case KEY_ENTER:
                *out_x = cur_x;
                *out_y = cur_y;
                return 1;
        }
    }
}
void run_tui_mode() {
    init_tui();
    
    int selected_menu_idx = 0;
    char status_msg[128] = "Welcome to 2D Graphics Editor TUI!";
    int status_color = 3;
    
    while (1) {
        check_term_size();
        create_windows();
        
        wclear(title_win);
        wattron(title_win, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(title_win, 0, 2, "=== 2D CHARACTER GRAPHICS EDITOR TUI ===");
        wattroff(title_win, COLOR_PAIR(1) | A_BOLD);
        wrefresh(title_win);
        
        render_canvas();
        display_canvas_tui();
        display_sidebar_menu(selected_menu_idx);
        
        wclear(status_win);
        box(status_win, 0, 0);
        wattron(status_win, COLOR_PAIR(status_color));
        mvwprintw(status_win, 1, 2, "%s", status_msg);
        wattroff(status_win, COLOR_PAIR(status_color));
        wrefresh(status_win);
        
        int ch = getch();
        if (ch == KEY_RESIZE) {
            continue;
        }
        
        int selected = -1;
        switch (ch) {
            case KEY_UP:
                selected_menu_idx = (selected_menu_idx - 1 + 7) % 7;
                break;
            case KEY_DOWN:
                selected_menu_idx = (selected_menu_idx + 1) % 7;
                break;
            case '\n':
            case KEY_ENTER:
                selected = selected_menu_idx;
                break;
            case 'l': case 'L': selected = 0; break;
            case 'r': case 'R': selected = 1; break;
            case 'c': case 'C': selected = 2; break;
            case 't': case 'T': selected = 3; break;
            case 'd': case 'D': selected = 4; break;
            case 'x': case 'X': selected = 5; break;
            case 'q': case 'Q': selected = 6; break;
        }
        
        if (selected == -1) {
            continue;
        }
        
        if (selected == 0) {
            if (num_shapes >= MAX_SHAPES) {
                snprintf(status_msg, sizeof(status_msg), "Error: Maximum shapes limit reached (%d)!", MAX_SHAPES);
                status_color = 5;
                continue;
            }
            
            int x1, y1, x2, y2;
            snprintf(status_msg, sizeof(status_msg), "Line: Select start point (X1, Y1).");
            status_color = 3;
            
            int ok1 = pick_coordinates(&x1, &y1, 0, (Shape){0});
            if (ok1) {
                Shape temp_line;
                temp_line.type = SHAPE_LINE;
                temp_line.data.line.x1 = x1;
                temp_line.data.line.y1 = y1;
                temp_line.data.line.x2 = x1;
                temp_line.data.line.y2 = y1;
                
                snprintf(status_msg, sizeof(status_msg), "Line: Select end point (X2, Y2).");
                int ok2 = pick_coordinates(&x2, &y2, 1, temp_line);
                if (ok2) {
                    shapes[num_shapes].type = SHAPE_LINE;
                    shapes[num_shapes].data.line.x1 = x1;
                    shapes[num_shapes].data.line.y1 = y1;
                    shapes[num_shapes].data.line.x2 = x2;
                    shapes[num_shapes].data.line.y2 = y2;
                    num_shapes++;
                    snprintf(status_msg, sizeof(status_msg), "Line added: (%d,%d) to (%d,%d)", x1, y1, x2, y2);
                    status_color = 3;
                } else {
                    snprintf(status_msg, sizeof(status_msg), "Line drawing cancelled.");
                    status_color = 5;
                }
            } else {
                snprintf(status_msg, sizeof(status_msg), "Line drawing cancelled.");
                status_color = 5;
            }
            
        } else if (selected == 1) {
            if (num_shapes >= MAX_SHAPES) {
                snprintf(status_msg, sizeof(status_msg), "Error: Maximum shapes limit reached (%d)!", MAX_SHAPES);
                status_color = 5;
                continue;
            }
            
            int x1, y1, x2, y2;
            snprintf(status_msg, sizeof(status_msg), "Rectangle: Select bottom-left corner.");
            status_color = 3;
            
            int ok1 = pick_coordinates(&x1, &y1, 0, (Shape){0});
            if (ok1) {
                Shape temp_rect;
                temp_rect.type = SHAPE_RECTANGLE;
                temp_rect.data.rect.x = x1;
                temp_rect.data.rect.y = y1;
                temp_rect.data.rect.width = 1;
                temp_rect.data.rect.height = 1;
                
                snprintf(status_msg, sizeof(status_msg), "Rectangle: Select opposite corner.");
                int ok2 = pick_coordinates(&x2, &y2, 1, temp_rect);
                if (ok2) {
                    int rx = x1 < x2 ? x1 : x2;
                    int ry = y1 < y2 ? y1 : y2;
                    int rw = abs(x2 - x1) + 1;
                    int rh = abs(y2 - y1) + 1;
                    
                    shapes[num_shapes].type = SHAPE_RECTANGLE;
                    shapes[num_shapes].data.rect.x = rx;
                    shapes[num_shapes].data.rect.y = ry;
                    shapes[num_shapes].data.rect.width = rw;
                    shapes[num_shapes].data.rect.height = rh;
                    num_shapes++;
                    snprintf(status_msg, sizeof(status_msg), "Rectangle added: corner (%d,%d), size %dx%d", rx, ry, rw, rh);
                    status_color = 3;
                } else {
                    snprintf(status_msg, sizeof(status_msg), "Rectangle drawing cancelled.");
                    status_color = 5;
                }
            } else {
                snprintf(status_msg, sizeof(status_msg), "Rectangle drawing cancelled.");
                status_color = 5;
            }
            
        } else if (selected == 2) {
            if (num_shapes >= MAX_SHAPES) {
                snprintf(status_msg, sizeof(status_msg), "Error: Maximum shapes limit reached (%d)!", MAX_SHAPES);
                status_color = 5;
                continue;
            }
            
            int cx, cy, x2, y2;
            snprintf(status_msg, sizeof(status_msg), "Circle: Select center point.");
            status_color = 3;
            
            int ok1 = pick_coordinates(&cx, &cy, 0, (Shape){0});
            if (ok1) {
                Shape temp_circle;
                temp_circle.type = SHAPE_CIRCLE;
                temp_circle.data.circle.cx = cx;
                temp_circle.data.circle.cy = cy;
                temp_circle.data.circle.r = 0;
                
                snprintf(status_msg, sizeof(status_msg), "Circle: Move cursor to define radius.");
                int ok2 = pick_coordinates(&x2, &y2, 1, temp_circle);
                if (ok2) {
                    double dist = sqrt((x2 - cx)*(x2 - cx) + (y2 - cy)*(y2 - cy));
                    int r = (int)(dist + 0.5);
                    
                    shapes[num_shapes].type = SHAPE_CIRCLE;
                    shapes[num_shapes].data.circle.cx = cx;
                    shapes[num_shapes].data.circle.cy = cy;
                    shapes[num_shapes].data.circle.r = r;
                    num_shapes++;
                    snprintf(status_msg, sizeof(status_msg), "Circle added: Center (%d,%d), Radius %d", cx, cy, r);
                    status_color = 3;
                } else {
                    snprintf(status_msg, sizeof(status_msg), "Circle drawing cancelled.");
                    status_color = 5;
                }
            } else {
                snprintf(status_msg, sizeof(status_msg), "Circle drawing cancelled.");
                status_color = 5;
            }
            
        } else if (selected == 3) {
            if (num_shapes >= MAX_SHAPES) {
                snprintf(status_msg, sizeof(status_msg), "Error: Maximum shapes limit reached (%d)!", MAX_SHAPES);
                status_color = 5;
                continue;
            }
            
            int x1, y1, x2, y2, x3, y3;
            snprintf(status_msg, sizeof(status_msg), "Triangle: Select first vertex.");
            status_color = 3;
            
            int ok1 = pick_coordinates(&x1, &y1, 0, (Shape){0});
            if (ok1) {
                Shape temp_line;
                temp_line.type = SHAPE_LINE;
                temp_line.data.line.x1 = x1;
                temp_line.data.line.y1 = y1;
                temp_line.data.line.x2 = x1;
                temp_line.data.line.y2 = y1;
                
                snprintf(status_msg, sizeof(status_msg), "Triangle: Select second vertex.");
                int ok2 = pick_coordinates(&x2, &y2, 1, temp_line);
                if (ok2) {
                    Shape temp_tri;
                    temp_tri.type = SHAPE_TRIANGLE;
                    temp_tri.data.triangle.x1 = x1;
                    temp_tri.data.triangle.y1 = y1;
                    temp_tri.data.triangle.x2 = x2;
                    temp_tri.data.triangle.y2 = y2;
                    temp_tri.data.triangle.x3 = x2;
                    temp_tri.data.triangle.y3 = y2;
                    
                    snprintf(status_msg, sizeof(status_msg), "Triangle: Select third vertex.");
                    int ok3 = pick_coordinates(&x3, &y3, 1, temp_tri);
                    if (ok3) {
                        shapes[num_shapes].type = SHAPE_TRIANGLE;
                        shapes[num_shapes].data.triangle.x1 = x1;
                        shapes[num_shapes].data.triangle.y1 = y1;
                        shapes[num_shapes].data.triangle.x2 = x2;
                        shapes[num_shapes].data.triangle.y2 = y2;
                        shapes[num_shapes].data.triangle.x3 = x3;
                        shapes[num_shapes].data.triangle.y3 = y3;
                        num_shapes++;
                        snprintf(status_msg, sizeof(status_msg), "Triangle added successfully.");
                        status_color = 3;
                    } else {
                        snprintf(status_msg, sizeof(status_msg), "Triangle drawing cancelled.");
                        status_color = 5;
                    }
                } else {
                    snprintf(status_msg, sizeof(status_msg), "Triangle drawing cancelled.");
                    status_color = 5;
                }
            } else {
                snprintf(status_msg, sizeof(status_msg), "Triangle drawing cancelled.");
                status_color = 5;
            }
            
        } else if (selected == 4) {
            if (num_shapes == 0) {
                snprintf(status_msg, sizeof(status_msg), "No shapes to delete.");
                status_color = 5;
                continue;
            }
            
            int del_idx = 0;
            show_highlight = 1;
            
            while (1) {
                highlight_shape_idx = del_idx;
                render_canvas_with_highlight(del_idx);
                display_canvas_tui();
                display_sidebar_shapes(del_idx);
                
                wclear(status_win);
                box(status_win, 0, 0);
                wattron(status_win, COLOR_PAIR(7));
                mvwprintw(status_win, 1, 2, "Select shape: Up/Down arrow | Enter: Delete | ESC: Cancel");
                wattroff(status_win, COLOR_PAIR(7));
                wrefresh(status_win);
                
                int key = getch();
                if (key == KEY_RESIZE) {
                    check_term_size();
                    create_windows();
                    continue;
                }
                
                if (key == KEY_UP) {
                    del_idx = (del_idx - 1 + num_shapes) % num_shapes;
                } else if (key == KEY_DOWN) {
                    del_idx = (del_idx + 1) % num_shapes;
                } else if (key == 27 || key == 'q' || key == 'Q') {
                    show_highlight = 0;
                    snprintf(status_msg, sizeof(status_msg), "Delete cancelled.");
                    status_color = 5;
                    break;
                } else if (key == '\n' || key == KEY_ENTER) {
                    for (int i = del_idx; i < num_shapes - 1; i++) {
                        shapes[i] = shapes[i + 1];
                    }
                    num_shapes--;
                    show_highlight = 0;
                    snprintf(status_msg, sizeof(status_msg), "Deleted shape at index %d.", del_idx);
                    status_color = 3;
                    break;
                }
            }
            
        } else if (selected == 5) {
            wclear(status_win);
            box(status_win, 0, 0);
            wattron(status_win, COLOR_PAIR(5));
            mvwprintw(status_win, 1, 2, "Are you sure you want to delete ALL shapes? (y/n)");
            wattroff(status_win, COLOR_PAIR(5));
            wrefresh(status_win);
            
            while (1) {
                int key = getch();
                if (key == KEY_RESIZE) {
                    check_term_size();
                    create_windows();
                    
                    wclear(status_win);
                    box(status_win, 0, 0);
                    wattron(status_win, COLOR_PAIR(5));
                    mvwprintw(status_win, 1, 2, "Are you sure you want to delete ALL shapes? (y/n)");
                    wattroff(status_win, COLOR_PAIR(5));
                    wrefresh(status_win);
                    continue;
                }
                if (key == 'y' || key == 'Y') {
                    num_shapes = 0;
                    snprintf(status_msg, sizeof(status_msg), "All shapes deleted. Canvas cleared.");
                    status_color = 3;
                    break;
                } else if (key == 'n' || key == 'N' || key == 27) {
                    snprintf(status_msg, sizeof(status_msg), "Clear cancelled.");
                    status_color = 5;
                    break;
                }
            }
            
        } else if (selected == 6) {
            endwin();
            exit(0);
        }
    }
}
