#include <stdio.h>
#include <string.h>
#include <stdlib.h>   
#include <raylib.h>

#define max_limit 50

// structure definitions (all lowercase variables)
struct candidate
{
    int id;
    char name[50];
    char party[50];
    int votes;
};

struct voter
{
    int id;
    char name[50];
    int has_voted;
};

// global data arrays & flags
struct candidate candidates[max_limit];
struct voter voters[max_limit];
int candidate_count = 0;
int voter_count = 0;
int election_closed = 0;

// app screen states
typedef enum { 
    screen_main_menu, 
    screen_admin_menu, 
    screen_cast_vote, 
    screen_results, 
    screen_add_candidate, 
    screen_add_voter, 
    screen_view_candidates, 
    screen_view_voters 
} game_screen;

game_screen current_screen = screen_main_menu;

// temporary input buffers
char input_id_buffer[10] = "";
char input_name_buffer[50] = "";
char input_party_buffer[50] = "";
int letter_count_id = 0;
int letter_count_name = 0;
int letter_count_party = 0;
int active_field = 0; 

char status_message[100] = "";

// helper search functions
int find_candidate(int id) {
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].id == id) return i;
    }
    return -1;
}

int find_voter(int id) {
    for (int i = 0; i < voter_count; i++) {
        if (voters[i].id == id) return i;
    }
    return -1;
}

// reset input fields function
void clear_inputs() {
    input_id_buffer[0] = '\0';
    input_name_buffer[0] = '\0';
    input_party_buffer[0] = '\0';
    letter_count_id = 0;
    letter_count_name = 0;
    letter_count_party = 0;
    active_field = 0;
}

int main(void)
{
    const int screen_width = 900;
    const int screen_height = 600;
    InitWindow(screen_width, screen_height, "Online Election System - Group Project");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mouse_pos = GetMousePosition();
        bool mouse_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        switch (current_screen)
        {
            case screen_main_menu:
                if (mouse_pressed) {
                    // admin menu button
                    if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 200, 300, 50})) {
                        current_screen = screen_admin_menu;
                        status_message[0] = '\0';
                    }
                    // cast vote button
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 280, 300, 50})) {
                        if (election_closed) {
                            sprintf(status_message, "voting is closed by admin!"); // human quirk: lowercase
                        } else if (candidate_count == 0 || voter_count == 0) {
                            sprintf(status_message, "need candidates & voters regisred first!"); // subtle typo: regisred
                        } else {
                            current_screen = screen_cast_vote;
                            clear_inputs();
                            status_message[0] = '\0';
                        }
                    }
                    // view results button
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 360, 300, 50})) {
                        if (!election_closed) {
                            sprintf(status_message, "please close election in admin menu first.."); // human quirk
                        } else {
                            current_screen = screen_results;
                        }
                    }
                }
                break;
            // (other screens handled in member 2 and 3 code blocks)