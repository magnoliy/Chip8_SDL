void Execute(int IPF) {

for (inst = 0; inst < IPF; inst++) {
	if (PC > 0xFFF) {return;}
	
	/*printf("Stack:\n");
	for (i = 0; i <= 15; i++) {
		printf("0x%01X = %03X  ", i, stack[i]);
	}
	printf("\n");*/	
	
	printf("\nstack top: %03X  index: %03X\n", stack[0], I);

	printf("registers\n");
	for (i = 0; i < 16; i++) {
		printf("V%01X: %02X   ", i, V[i]);
	}
	printf("\n");

	opcode_high = mem[PC];
	opcode_low = mem[PC+1];
	opcode = (opcode_high << 8) | opcode_low;

	n1 = (opcode & 0xF000) >> 12;
	n2 = (opcode & 0x0F00) >> 8;
	n3 = (opcode & 0x00F0) >> 4;
	n4 = (opcode & 0x000F);

	printf("PC: %03X, opcode: %04X\n", PC, opcode);
	//printf("Opcode: %04X\n", opcode);

	
	//opcode implementations
	
	switch (n1) {
		case 0x0:
				if (opcode == 0x00EE) {
					//00EE - RET
					//decrement SP (index of stack top)
					SP--;
					PC = stack[SP];

					printf("RET\n");
						
				}
				else if (opcode == 0x00E0){
					//00E0 - CLS
					////Clear the display
					fill = 1;
					printf("CLS\n");
					PC += 0x2;
				}
			
			break;
		
		case 0x1:
			//1nnn - JP addr
			
			PC = (opcode & 0x0FFF);
			printf("JP %03X\n", PC);
			break;

		case 0x2:
			//2nnn - CALL addr
		
                        PC += 0x2;
			stack[SP] = PC;

			SP++;
			
			PC = (opcode & 0x0FFF);
			
			printf("CALL %03X\n", PC);
                        break;

                case 0x3:
			//3xkk - SE Vx, byte
                       	Vx = n2;
			if (V[Vx] == (opcode & 0x00FF)){
				PC += 0x2;
			} 
			PC += 0x2;
			printf("SE V%01X, %02X\n", Vx, (opcode & 0x00FF));
		       	break;

		case 0x4:
			//4xkk - SNE Vx, byte
			Vx = n2;
                        if (V[Vx] != (opcode & 0x00FF)){
                                PC += 0x2;
                        }
                        PC += 0x2;
                        printf("SNE V%01X, %02X\n", Vx, (opcode & 0x00FF));
                        break;

                case 0x5:
			//5xy0 - SE Vx, Vy
                        Vx = n2;
                        Vy = n3;
			if (V[Vx] == V[Vy]){
                                PC += 0x2;
                        }
                        PC += 0x2;
                        printf("SE V%01X, V%01X\n", Vx, Vy);
			break;

		case 0x6:
			//6xkk - LD Vx, byte
			Vx = n2;
			//Fill specified register with kk
			V[Vx] = (opcode & 0x00FF);

			printf("LD V%01X %02X\n", Vx, V[Vx]);
			PC += 0x2;
                        break;

                case 0x7:
			//7xkk - ADD Vx, byte
			
                        Vx = n2;

			V[Vx] += (opcode & 0x00FF);
			
			printf("ADD V%01X, %02X\n", Vx, (opcode & 0x00FF));
                        PC += 0x2;
			break;

                case 0x8:
			switch(n4) {
				case 0x0:
					//8xy0 - LD Vx, Vy
					Vx = n2;
					Vy = n3;

					V[Vx] = V[Vy];
					printf("LD V%01X, V%01X\n", Vx, Vy);
					break;
				case 0x1:
					//8xy1 - OR Vx, Vy
					Vx = n2;
                                        Vy = n3;

                                        V[Vx] |= V[Vy];
                                        printf("OR V%01X, V%01X", Vx, Vy);
                                        break;
				case 0x2:
					//8xy2 - AND Vx, Vy
					Vx = n2;
                                        Vy = n3;

                                        V[Vx] &= V[Vy];
                                        printf("AND V%01X, V%01X", Vx, Vy);
                                        break;
				case 0x3:
					//8xy3 - XOR Vx, Vy
					Vx = n2;
                                        Vy = n3;
					
                                        V[Vx] ^= V[Vy];
                                        printf("XOR V%01X, V%01X", Vx, Vy);
                                        break;
                                case 0x4:
					//8xy4 - ADD Vx, Vy
					Vx = n2;
                                        Vy = n3;	

					oldVx = V[Vx];
                                        oldVy = V[Vy];

                                        V[Vx] = oldVx + oldVy;


                                        //carry bit to VF
					if (oldVx + oldVy > 0xFF) VF = 1;
					else VF = 0;

                                        printf("ADD V%01X, V%01X", Vx, Vy);
                                        break;
                                case 0x5:
					//8xy5 - SUB Vx, Vy
					Vx = n2;
                                        Vy = n3;	

					oldVx = V[Vx];
					oldVy = V[Vy];

					V[Vx] = oldVx - oldVy;
					
                                        //set borrow bit if VY > VX
                                        if (oldVx >= oldVy) VF = 1;
                                        else VF = 0;

                                        printf("SUB V%01X, V%01X", Vx, Vy);
                                        break;
				case 0x6:
					//8xy6 - SHR Vx {, Vy}
					//Take second and third char, turn into hex number
					Vx = n2;
                                        //Vy = n3;

                                        //set Vx to Vy
                                        //V[Vx] = V[Vy];
                                        oldVx = V[Vx];

                                        //left shift Vx by 1
                                        V[Vx] >>= 1;

					//set VF to most significant bit of Vx (mask everything but first bit, shift it to right)
					VF = oldVx & 0x1;

                                        printf("SHR V%01X\n", Vx);
                                        
                                        break;
                                case 0x7:
					//8xy7 - SUBN Vx, Vy
					Vx = n2;
                                        Vy = n3;	

					oldVx = V[Vx];
                                        oldVy = V[Vy];

                                        V[Vx] = oldVy - oldVx;

					//carry bit to VF
                                        if (oldVx <= oldVy) VF = 1;
                                        else VF = 0;

                                        printf("SUBN V%01X, V%01X", Vx, Vy);
					break;
                                case 0xE:
					//8xyE - SHL Vx {, Vy}
					//Take second and third char, turn into hex number
					Vx = n2;
					//Vy = n3;
				
					oldVx = V[Vx];

					//left shift Vx by 1
                                        V[Vx]<<=1;					

					//set Vx to Vy
					//V[Vx] = V[Vy];
					//set VF to most significant bit of Vx (mask everything but first bit, shift it to right)
					VF = (oldVx & 0x80) >> 7;

					printf("SHL V%01X\n", Vx);
					
                                        break;
			}
				PC += 0x2;
                        break;

                case 0x9:
			//9xy0 - SNE Vx, Vy
                        Vx = n2;
                        Vy = n3;
			
			if (V[Vx] != V[Vy]){
                                PC += 0x2;
                        }
                        PC += 0x2;
                        printf("SNE V%01X, V%01X\n", Vx, Vy);
			break;

                case 0xA:
			//Annn - LD I, addr
			
			I = (opcode & 0x0FFF);
			
			printf("LD I, %03X\n", I);
			
			PC += 0x2;
			
                        break;

                case 0xB:
			//Bnnn - JP V0, addr
                        
			//Turn last 3 chars into int, make PC equal to it + V0
		       	PC = (opcode & 0x0FFF) + V[0];	
			printf("JP V0 %03X\n", (opcode & 0x0FFF));
			break;
			
		case 0xC:
			//Cxkk - RND Vx, byte
			Vx = n2;

			//generate random number with ticks as the seed
			srand(SDL_GetTicks());

			V[Vx] = (rand() & 0xFF) & (opcode & 0x00FF);

			printf("RND V%01X, %02X", Vx, (opcode & 0x00FF));
			PC += 0x2;
			break;

                case 0xD:
			//Dxyn - DRW Vx, Vy, nibble
			
			//x, y and n to hex int
			Vx = n2;
			Vy = n3;
			N = n4;

			sX = V[Vx];
			sY = V[Vy];

			Draw(sX, sY, N, I);

			printf("DRW V%01X, V%01X, %01X\n", Vx, Vy, N);
			PC += 0x2;

                        break;

                case 0xE:
			switch(n3) {
				case 0x9:
					//Ex9E - SKP Vx
					Vx = n2;
					if (keypad[V[Vx]]) {
 					   PC += 4;  // skip next instruction (2 bytes) + advance current opcode (2 bytes)
					} else {
					    PC += 2;  // just advance current opcode
					}
					printf("SKP V%01X\n", Vx);
					break;
				case 0xA:
					//ExA1 - SKNP Vx
					Vx = n2;
                                        if (!keypad[V[Vx]]) {
 					   PC += 4;  // skip next instruction (2 bytes) + advance current opcode (2 bytes)
					} else {
					   PC += 2;  // just advance current opcode
					}
					printf("SKPN V%01X\n", Vx);
					break;
			}
                        break;

                case 0xF:
			switch(n3) {
				case 0x0:
					switch(n4) {
						case 0x7:
							//Fx07 - LD Vx, DT
							Vx = n2;
							V[Vx] = DT;
							printf("LD V%01X, DT\n", Vx);
							PC += 0x2;
							break;
						case 0xA:
							//Fx0A - LD Vx, K
							Vx = n2;
							for (i = 0; i < 16; i++) {
								if (keypad[i]) {
									key = i;
									break;
								}
							}
							
							if (keypad[i] == 0 && key <= 0xF) {
								V[Vx] = key;	
								key = 0x10;
								PC += 0x2;
								printf("LD V%01X, K\n", Vx);
							}
							break;
					}
					break;
				case 0x1:
					switch(n4) {
						case 0x5:
							//Fx15 - LD DT, Vx;
							Vx = n2;
							DT = V[Vx];
							printf("LD DT, V%01X\n", Vx);
							PC += 0x2;	
							break;
						case 0x8:
							//Fx18 - LD ST, Vx
							Vx = n2;
                                                        ST = V[Vx];
                                                        printf("LD ST, V%01X\n", Vx);
                                                        PC += 0x2;
							break;
						case 0xE:
							//Fx1E - ADD I, Vx
							Vx = n2;
							I += V[Vx];

							printf("ADD I, V%01X\n", Vx);
							PC += 0x2;
							break;
							
					}
					break;
				case 0x2:
					//Fx29 - LD F, Vx
					Vx = n2;
					I = V[Vx] * 0x5;
					printf("LD F, V%01X\n", Vx);
					PC += 0x2;
					break;
				case 0x3:
					//Fx33 - LD B, Vx
					Vx = n2;
					h = V[Vx] / 100;
					t = (V[Vx] - (h * 100)) / 10;
					o = V[Vx] - (h * 100) - (t * 10);

					mem[I] = h;
					mem[I+1] = t;
					mem[I+2] = o;

					PC += 0x2;
					printf("LD B, V%01X\n", Vx);
					break;
				case 0x5:
					//Fx55 - LD [I], Vx
					Vx = n2;
					for (i = 0; i <= Vx; i++) {
                                                mem[I + i] = V[i];

                                        }
                                        printf("LD [I], V%01X\n", Vx);
                                        PC += 0x2;
					break;
				case 0x6:
					//Fx65 - LD Vx, [I]
					Vx = n2;
					for (i = 0; i <= Vx; i++) {
						V[i]= mem[I + i];

					}
					printf("LD V%01X, [I]\n", Vx);
					PC += 0x2;		
					break;
			}

                        break;

		}
	}			
}




