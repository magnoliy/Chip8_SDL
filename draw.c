void Draw (int sX, int sY, int N, unsigned short I) {
	VF = 0;
	printf("Sprite at I = 0x%03X:\n", I);
		for (int i = 0; i < N; i++) {
    		printf("0x%03X: %02X\n", I + i, mem[I + i]);
	}

	for (row = 0; row < N; row++) {

                        //load a row of pixels
                        bits = mem[I + row];
			
			cY = (sY + row) % height;

                        for (col = 0; col < 8; col++) {
				cX = (sX + col) % width;


                                //get current pixel in display array
                                curr_pixel = display[cX][cY];
                                //left shift to get the bit we want, mask the rest away
                                curr_bit = (bits >> (7 - col)) & 1;
                                //shitty XOR
                                if (curr_bit) {
                                        if (curr_pixel) {
                                                display[cX][cY] = 0;
                                                VF = 1;
                                        }
                                        else {
                                                display[cX][cY] = 1;
                                        }
                                }


                        }

                }
}

SDL_Surface* Render (SDL_Surface *psurface) {
		//screen clear
		if (fill == 1) {
			SDL_FillRect(psurface, NULL, cBG);
			fill = 0;
			memset(display, 0, sizeof(display));
			return psurface;
		}

		//render to surface
		for (cX = 0; cX < width; cX++) {
                	pixel.x = cX * scale;
                	for (cY = 0; cY < height; cY++){
                        	pixel.y = cY * scale;
                        	//draw pixel
                        	if (display[cX][cY] == 1) {
					SDL_FillRect(psurface, &pixel, cPixel);
               			 }
				if (display[cX][cY] == 0) {
					SDL_FillRect(psurface, &pixel, cBG);
				}
        		}
		}	

		return psurface;
}
