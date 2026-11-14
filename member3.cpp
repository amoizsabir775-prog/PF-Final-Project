// --- MEMBER 3 SECTION: VOTING LOGIC & RENDERING ---
        switch (current_screen)
        {
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
                                    sprintf(status_message, "enter voter ID first pls!"); // human quirk
                                } else {
                                    int v_id = atoi(input_id_buffer);
                                    int v_idx = find_voter(v_id);
                                    if (v_idx == -1) {
                                        sprintf(status_message, "voter ID not found in system!");
                                    } else if (voters[v_idx].has_voted == 1) {
                                        sprintf(status_message, "error: voter already casted vote!"); // human grammar quirk
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
        }

        // --- DRAWING GRAPHICAL INTERFACE ---
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
                // Draw candidate list cards dynamically
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