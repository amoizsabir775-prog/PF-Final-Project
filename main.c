#include <stdio.h>
#include <string.h>
#include <stdlib.h>    
#include "raylib.h"

#define max_limit 50

// structure definitions
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

        // --- INPUT & LOGIC HANDLING SWITCH ---
        switch (current_screen)
        {
            case screen_main_menu:
                if (mouse_pressed) {
                    if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 200, 300, 50})) {
                        current_screen = screen_admin_menu;
                        status_message[0] = '\0';
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 280, 300, 50})) {
                        if (election_closed) {
                            sprintf(status_message, "voting is closed by admin!");
                        } else if (candidate_count == 0 || voter_count == 0) {
                            sprintf(status_message, "need candidates & voters regisred first!");
                        } else {
                            current_screen = screen_cast_vote;
                            clear_inputs();
                            status_message[0] = '\0';
                        }
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 360, 300, 50})) {
                        if (!election_closed) {
                            sprintf(status_message, "please close election in admin menu first..");
                        } else {
                            current_screen = screen_results;
                        }
                    }
                }
                break;

            case screen_admin_menu:
                if (mouse_pressed) {
                    if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 150, 300, 45})) {
                        current_screen = screen_add_candidate; clear_inputs(); status_message[0] = '\0';
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 210, 300, 45})) {
                        current_screen = screen_add_voter; clear_inputs(); status_message[0] = '\0';
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 270, 300, 45})) {
                        current_screen = screen_view_candidates;
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 330, 300, 45})) {
                        current_screen = screen_view_voters;
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 390, 300, 45})) {
                        election_closed = 1;
                        sprintf(status_message, "Election closed successfully.");
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){300, 450, 300, 45})) {
                        current_screen = screen_main_menu;
                        status_message[0] = '\0';
                    }
                }
                break;

            case screen_add_candidate:
                if (IsKeyPressed(KEY_TAB)) active_field = (active_field % 3) + 1;
                
                if (active_field == 1) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= '0') && (key <= '9') && (letter_count_id < 8)) {
                            input_id_buffer[letter_count_id] = (char)key;
                            input_id_buffer[letter_count_id+1] = '\0';
                            letter_count_id++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && letter_count_id > 0) {
                        letter_count_id--;
                        input_id_buffer[letter_count_id] = '\0';
                    }
                } else if (active_field == 2) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= 32) && (key <= 125) && (letter_count_name < 45)) {
                            input_name_buffer[letter_count_name] = (char)key;
                            input_name_buffer[letter_count_name+1] = '\0';
                            letter_count_name++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && letter_count_name > 0) {
                        letter_count_name--;
                        input_name_buffer[letter_count_name] = '\0';
                    }
                } else if (active_field == 3) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= 32) && (key <= 125) && (letter_count_party < 45)) {
                            input_party_buffer[letter_count_party] = (char)key;
                            input_party_buffer[letter_count_party+1] = '\0';
                            letter_count_party++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && letter_count_party > 0) {
                        letter_count_party--;
                        input_party_buffer[letter_count_party] = '\0';
                    }
                }

                if (mouse_pressed) {
                    if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 180, 400, 35})) active_field = 1;
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 250, 400, 35})) active_field = 2;
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 320, 400, 35})) active_field = 3;
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 385, 180, 40})) {
                        if (election_closed) {
                            sprintf(status_message, "registration is closed now.");
                        } else if (letter_count_id == 0 || letter_count_name == 0 || letter_count_party == 0) {
                            sprintf(status_message, "Plz fill out all fields!");
                        } else if (candidate_count >= max_limit) {
                            sprintf(status_message, "Candidate list is full!");
                        } else {
                            int id = atoi(input_id_buffer);
                            if (find_candidate(id) != -1) {
                                sprintf(status_message, "Candidate ID already exists!");
                            } else {
                                candidates[candidate_count].id = id;
                                strcpy(candidates[candidate_count].name, input_name_buffer);
                                strcpy(candidates[candidate_count].party, input_party_buffer);
                                candidates[candidate_count].votes = 0;
                                candidate_count++;
                                sprintf(status_message, "candidate added successfully!");
                                clear_inputs();
                            }
                        }
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){470, 385, 180, 40})) {
                        current_screen = screen_admin_menu;
                    }
                }
                break;

            case screen_add_voter:
                if (IsKeyPressed(KEY_TAB)) active_field = (active_field % 2) + 1;

                if (active_field == 1) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= '0') && (key <= '9') && (letter_count_id < 8)) {
                            input_id_buffer[letter_count_id] = (char)key;
                            input_id_buffer[letter_count_id+1] = '\0';
                            letter_count_id++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && letter_count_id > 0) {
                        letter_count_id--;
                        input_id_buffer[letter_count_id] = '\0';
                    }
                } else if (active_field == 2) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= 32) && (key <= 125) && (letter_count_name < 45)) {
                            input_name_buffer[letter_count_name] = (char)key;
                            input_name_buffer[letter_count_name+1] = '\0';
                            letter_count_name++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && letter_count_name > 0) {
                        letter_count_name--;
                        input_name_buffer[letter_count_name] = '\0';
                    }
                }

                if (mouse_pressed) {
                    if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 200, 400, 35})) active_field = 1;
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 280, 400, 35})) active_field = 2;
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){250, 350, 180, 40})) {
                        if (election_closed) {
                            sprintf(status_message, "registration is closed.");
                        } else if (letter_count_id == 0 || letter_count_name == 0) {
                            sprintf(status_message, "please fill all fields.");
                        } else if (voter_count >= max_limit) {
                            sprintf(status_message, "voter list full!");
                        } else {
                            int id = atoi(input_id_buffer);
                            if (find_voter(id) != -1) {
                                sprintf(status_message, "voter id already registered!");
                            } else {
                                voters[voter_count].id = id;
                                strcpy(voters[voter_count].name, input_name_buffer);
                                voters[voter_count].has_voted = 0;
                                voter_count++;
                                sprintf(status_message, "voter registered successfully!");
                                clear_inputs();
                            }
                        }
                    }
                    else if (CheckCollisionPointRec(mouse_pos, (Rectangle){470, 350, 180, 40})) {
                        current_screen = screen_admin_menu;
                    }
                }
                break;

            case screen_cast_vote:
                {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= '0') && (key <= '9') && (letter_count_id < 8)) {
                            input_id_buffer[letter_count_id] = (char)key;
                            input_id_buffer[letter_count_id+1] = '\0';
                            letter_count_id++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && letter_count_id > 0) {
                        letter_count_id--;
                        input_id_buffer[letter_count_id] = '\0';
                    }

                    if (mouse_pressed) {
                        if (CheckCollisionPointRec(mouse_pos, (Rectangle){50, 520, 120, 35})) {
                            current_screen = screen_main_menu;
                        }

                        for (int i = 0; i < candidate_count; i++) {
                            Rectangle card_rect = { 480, (float)(120 + i * 55), 380, 45 };
                            if (CheckCollisionPointRec(mouse_pos, card_rect)) {
                                if (letter_count_id == 0) {
                                    sprintf(status_message, "enter voter ID first pls!");
                                } else {
                                    int v_id = atoi(input_id_buffer);
                                    int v_idx = find_voter(v_id);
                                    if (v_idx == -1) {
                                        sprintf(status_message, "voter ID not found in system!");
                                    } else if (voters[v_idx].has_voted == 1) {
                                        sprintf(status_message, "error: voter already casted vote!");
                                    } else {
                                        candidates[i].votes++;
                                        voters[v_idx].has_voted = 1;
                                        sprintf(status_message, "vote cast successfully to %s!", candidates[i].name);
                                        clear_inputs();
                                    }
                                }
                            }
                        }
                    }
                }
                break;

            case screen_view_candidates:
            case screen_view_voters:
            case screen_results:
                if (mouse_pressed) {
                    if (CheckCollisionPointRec(mouse_pos, (Rectangle){380, 520, 140, 40})) {
                        current_screen = (current_screen == screen_results) ? screen_main_menu : screen_admin_menu;
                    }
                }
                break;

            default:
                break;
        }

        // --- DRAWING GRAPHICAL INTERFACE SWITCH ---
        BeginDrawing();
        ClearBackground(GetColor(0x181824FF));

        switch (current_screen)
        {
            case screen_main_menu:
                DrawText("ONLINE ELECTION SYSTEM", 220, 80, 30, RAYWHITE);
                
                DrawRectangleRec((Rectangle){300, 200, 300, 50}, LIGHTGRAY);
                DrawText("1. Administrator Menu", 335, 215, 20, DARKBLUE);

                DrawRectangleRec((Rectangle){300, 280, 300, 50}, LIGHTGRAY);
                DrawText("2. Cast Your Vote", 355, 295, 20, DARKBLUE);

                DrawRectangleRec((Rectangle){300, 360, 300, 50}, LIGHTGRAY);
                DrawText("3. View Results", 380, 375, 20, DARKBLUE);

                if (strlen(status_message) > 0) {
                    DrawText(status_message, screen_width/2 - MeasureText(status_message, 16)/2, 450, 16, YELLOW);
                }
                break;

            case screen_admin_menu:
                DrawText("ADMINISTRATOR PANEL", 295, 60, 28, RAYWHITE);

                DrawRectangleRec((Rectangle){300, 150, 300, 45}, LIGHTGRAY);
                DrawText("Add Candidate", 380, 162, 18, DARKBLUE);

                DrawRectangleRec((Rectangle){300, 210, 300, 45}, LIGHTGRAY);
                DrawText("Register Voter", 380, 222, 18, DARKBLUE);

                DrawRectangleRec((Rectangle){300, 270, 300, 45}, LIGHTGRAY);
                DrawText("Display Candidates", 360, 282, 18, DARKBLUE);

                DrawRectangleRec((Rectangle){300, 330, 300, 45}, LIGHTGRAY);
                DrawText("Display Voters", 380, 342, 18, DARKBLUE);

                DrawRectangleRec((Rectangle){300, 390, 300, 45}, MAROON);
                DrawText("Close Election", 380, 402, 18, RAYWHITE);

                DrawRectangleRec((Rectangle){300, 450, 300, 45}, DARKGRAY);
                DrawText("Back to Main Menu", 360, 462, 18, RAYWHITE);

                if (strlen(status_message) > 0) {
                    DrawText(status_message, screen_width/2 - MeasureText(status_message, 16)/2, 520, 16, YELLOW);
                }
                break;

            case screen_add_candidate:
                DrawText("ADD NEW CANDIDATE", 310, 60, 26, RAYWHITE);

                DrawText("Candidate ID:", 250, 150, 18, LIGHTGRAY);
                DrawRectangleRec((Rectangle){250, 180, 400, 35}, active_field == 1 ? WHITE : LIGHTGRAY);
                DrawText(input_id_buffer, 260, 188, 18, BLACK);

                DrawText("Candidate Name:", 250, 225, 18, LIGHTGRAY);
                DrawRectangleRec((Rectangle){250, 250, 400, 35}, active_field == 2 ? WHITE : LIGHTGRAY);
                DrawText(input_name_buffer, 260, 258, 18, BLACK);

                DrawText("Party Name:", 250, 295, 18, LIGHTGRAY);
                DrawRectangleRec((Rectangle){250, 320, 400, 35}, active_field == 3 ? WHITE : LIGHTGRAY);
                DrawText(input_party_buffer, 260, 328, 18, BLACK);

                DrawRectangleRec((Rectangle){250, 385, 180, 40}, DARKGREEN);
                DrawText("Submit", 310, 395, 18, RAYWHITE);

                DrawRectangleRec((Rectangle){470, 385, 180, 40}, DARKGRAY);
                DrawText("Cancel", 530, 395, 18, RAYWHITE);

                if (strlen(status_message) > 0) {
                    DrawText(status_message, screen_width/2 - MeasureText(status_message, 16)/2, 460, 16, YELLOW);
                }
                break;

            case screen_add_voter:
                DrawText("REGISTER NEW VOTER", 305, 100, 26, RAYWHITE);

                DrawText("Voter ID:", 250, 170, 18, LIGHTGRAY);
                DrawRectangleRec((Rectangle){250, 200, 400, 35}, active_field == 1 ? WHITE : LIGHTGRAY);
                DrawText(input_id_buffer, 260, 208, 18, BLACK);

                DrawText("Voter Name:", 250, 250, 18, LIGHTGRAY);
                DrawRectangleRec((Rectangle){250, 280, 400, 35}, active_field == 2 ? WHITE : LIGHTGRAY);
                DrawText(input_name_buffer, 260, 288, 18, BLACK);

                DrawRectangleRec((Rectangle){250, 350, 180, 40}, DARKGREEN);
                DrawText("Register", 305, 360, 18, RAYWHITE);

                DrawRectangleRec((Rectangle){470, 350, 180, 40}, DARKGRAY);
                DrawText("Cancel", 530, 360, 18, RAYWHITE);

                if (strlen(status_message) > 0) {
                    DrawText(status_message, screen_width/2 - MeasureText(status_message, 16)/2, 430, 16, YELLOW);
                }
                break;

            case screen_cast_vote:
                DrawText("CAST YOUR VOTE", 50, 50, 24, RAYWHITE);
                DrawText("1. Enter Voter ID:", 50, 120, 18, LIGHTGRAY);
                DrawRectangleRec((Rectangle){50, 150, 250, 35}, WHITE);
                DrawText(input_id_buffer, 60, 158, 18, BLACK);

                DrawText("2. Click Candidate to Vote:", 480, 80, 18, LIGHTGRAY);
                for (int i = 0; i < candidate_count; i++) {
                    DrawRectangleRec((Rectangle){480, (float)(120 + i * 55), 380, 45}, LIGHTGRAY);
                    char candidate_info[100];
                    sprintf(candidate_info, "%s (%s)", candidates[i].name, candidates[i].party);
                    DrawText(candidate_info, 495, 132 + i * 55, 16, DARKBLUE);
                }

                DrawRectangleRec((Rectangle){50, 520, 120, 35}, DARKGRAY);
                DrawText("Back", 90, 528, 16, RAYWHITE);

                if (strlen(status_message) > 0) {
                    DrawText(status_message, 50, 470, 16, YELLOW);
                }
                break;
                
            case screen_results:
                DrawText("ELECTION RESULTS", 320, 60, 26, RAYWHITE);
                for (int i = 0; i < candidate_count; i++) {
                    char res_str[150];
                    sprintf(res_str, "%d. %s [%s] -> Votes: %d", i+1, candidates[i].name, candidates[i].party, candidates[i].votes);
                    DrawText(res_str, 250, 140 + (i * 40), 18, RAYWHITE);
                }
                DrawRectangleRec((Rectangle){380, 520, 140, 40}, DARKGRAY);
                DrawText("Main Menu", 410, 530, 16, RAYWHITE);
                break;

            case screen_view_candidates:
                DrawText("REGISTERED CANDIDATES", 290, 50, 26, RAYWHITE);
                for(int i = 0; i < candidate_count; i++) {
                    char cand_txt[120];
                    sprintf(cand_txt, "ID: %d | Name: %s | Party: %s", candidates[i].id, candidates[i].name, candidates[i].party);
                    DrawText(cand_txt, 220, 110 + (i * 35), 18, LIGHTGRAY);
                }
                DrawRectangleRec((Rectangle){380, 520, 140, 40}, DARKGRAY);
                DrawText("Admin Menu", 405, 530, 16, RAYWHITE);
                break;

            case screen_view_voters:
                DrawText("REGISTERED VOTERS", 310, 50, 26, RAYWHITE);
                for(int i = 0; i < voter_count; i++) {
                    char voter_txt[120];
                    sprintf(voter_txt, "ID: %d | Name: %s | Voted: %s", voters[i].id, voters[i].name, voters[i].has_voted ? "Yes" : "No");
                    DrawText(voter_txt, 250, 110 + (i * 35), 18, LIGHTGRAY);
                }
                DrawRectangleRec((Rectangle){380, 520, 140, 40}, DARKGRAY);
                DrawText("Admin Menu", 405, 530, 16, RAYWHITE);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}