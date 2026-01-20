void update_keys () {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    keypad[0x1] = state[SDL_SCANCODE_1];
    keypad[0x2] = state[SDL_SCANCODE_2];
    keypad[0x3] = state[SDL_SCANCODE_3];
    keypad[0xC] = state[SDL_SCANCODE_4];

    keypad[0x4] = state[SDL_SCANCODE_Q];
    keypad[0x5] = state[SDL_SCANCODE_W];
    keypad[0x6] = state[SDL_SCANCODE_E];
    keypad[0xD] = state[SDL_SCANCODE_R];

    keypad[0x7] = state[SDL_SCANCODE_A];
    keypad[0x8] = state[SDL_SCANCODE_S];
    keypad[0x9] = state[SDL_SCANCODE_D];
    keypad[0xE] = state[SDL_SCANCODE_F];

    keypad[0xA] = state[SDL_SCANCODE_Z];
    keypad[0x0] = state[SDL_SCANCODE_X];
    keypad[0xB] = state[SDL_SCANCODE_C];
    keypad[0xF] = state[SDL_SCANCODE_V];

}

