#include <stdio.h>
#include <string.h>
#include <stdlib.h>   
#include <raylib.h>

// --- MEMBER 2 SECTION: ADMIN & REGISTRATION LOGIC ---
        switch (current_screen)
        {
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
                            sprintf(status_message, "Plz fill out all fields!"); // human quirk: Plz
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
            }