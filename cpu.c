void Execute(int IPF) {

for (inst = 0; inst < IPF; inst++) {
	/*printf("Stack:\n");
	for (i = 0; i <= 15; i++) {
		printf("0x%01X = %03X  ", i, stack[i]);
	}
	printf("\n");*/	
	
	/*printf("\nstack top: %03X  index: %03X\n", stack[0], I);

	printf("registers\n");
	for (i = 0; i < 16; i++) {
		printf("V%01X: %02X   ", i, V[i]);
	}
	printf("\n");*/

	opcode_high = mem[PC];
	opcode_low = mem[PC+1];
	//PC += 0x2;
	opcode = (opcode_high << 8) | opcode_low;

	n1 = (opcode & 0xF000) >> 12;
	n2 = (opcode & 0x0F00) >> 8;
	n3 = (opcode & 0x00F0) >> 4;
	n4 = (opcode & 0x000F);

	//printf("PC: %03X, opcode: %04X\n", PC, opcode);

	
	//opcode implementations
switch (n1) {
case 0x0:
if (opcode == 0x00EE) {
//00EE - RET
SP--;
PC = stack[SP];
} else if (opcode == 0x00E0) {
//00E0 - CLS
fill = 1;
PC += 0x2;
}
break;

case 0x1:
//1nnn - JP addr
PC = (opcode & 0x0FFF);
break;

case 0x2:
//2nnn - CALL addr
PC += 0x2;
stack[SP] = PC;
SP++;
PC = (opcode & 0x0FFF);
break;

case 0x3:
//3xkk - SE Vx, byte
Vx = n2;
if (V[Vx] == (opcode & 0x00FF)) PC += 0x2;
PC += 0x2;
break;

case 0x4:
//4xkk - SNE Vx, byte
Vx = n2;
if (V[Vx] != (opcode & 0x00FF)) PC += 0x2;
PC += 0x2;
break;

case 0x5:
//5xy0 - SE Vx, Vy
Vx = n2;
Vy = n3;
if (V[Vx] == V[Vy]) PC += 0x2;
PC += 0x2;
break;

case 0x6:
//6xkk - LD Vx, byte
Vx = n2;
V[Vx] = (opcode & 0x00FF);
PC += 0x2;
break;

case 0x7:
//7xkk - ADD Vx, byte
Vx = n2;
V[Vx] += (opcode & 0x00FF);
PC += 0x2;
break;

case 0x8:
switch (n4) {
case 0x0:
//8xy0 - LD Vx, Vy
Vx = n2;
Vy = n3;
V[Vx] = V[Vy];
break;

case 0x1:
//8xy1 - OR Vx, Vy
Vx = n2;
Vy = n3;
V[Vx] |= V[Vy];
break;

case 0x2:
//8xy2 - AND Vx, Vy
Vx = n2;
Vy = n3;
V[Vx] &= V[Vy];
break;

case 0x3:
//8xy3 - XOR Vx, Vy
Vx = n2;
Vy = n3;
V[Vx] ^= V[Vy];
break;

case 0x4:
//8xy4 - ADD Vx, Vy
Vx = n2;
Vy = n3;
oldVx = V[Vx];
oldVy = V[Vy];
V[Vx] = oldVx + oldVy;
VF = (oldVx + oldVy > 0xFF);
break;

case 0x5:
//8xy5 - SUB Vx, Vy
Vx = n2;
Vy = n3;
oldVx = V[Vx];
oldVy = V[Vy];
V[Vx] = oldVx - oldVy;
VF = (oldVx >= oldVy);
break;

case 0x6:
//8xy6 - SHR Vx {, Vy}
Vx = n2;
oldVx = V[Vx];
V[Vx] >>= 1;
VF = oldVx & 0x1;
break;

case 0x7:
//8xy7 - SUBN Vx, Vy
Vx = n2;
Vy = n3;
oldVx = V[Vx];
oldVy = V[Vy];
V[Vx] = oldVy - oldVx;
VF = (oldVx <= oldVy);
break;

case 0xE:
//8xyE - SHL Vx {, Vy}
Vx = n2;
oldVx = V[Vx];
V[Vx] <<= 1;
VF = (oldVx & 0x80) >> 7;
break;
}
PC += 0x2;
break;

case 0x9:
//9xy0 - SNE Vx, Vy
Vx = n2;
Vy = n3;
if (V[Vx] != V[Vy]) PC += 0x2;
PC += 0x2;
break;

case 0xA:
//Annn - LD I, addr
I = (opcode & 0x0FFF);
PC += 0x2;
break;

case 0xB:
//Bnnn - JP V0, addr
PC = (opcode & 0x0FFF) + V[0];
break;

case 0xC:
//Cxkk - RND Vx, byte
Vx = n2;
srand(SDL_GetTicks());
V[Vx] = (rand() & 0xFF) & (opcode & 0x00FF);
PC += 0x2;
break;

case 0xD:
//Dxyn - DRW Vx, Vy, nibble
Vx = n2;
Vy = n3;
N = n4;
sX = V[Vx];
sY = V[Vy];
Draw(sX, sY, N, I);
PC += 0x2;
break;

case 0xE:
switch (n3) {
case 0x9:
//Ex9E - SKP Vx
Vx = n2;
PC += keypad[V[Vx]] ? 4 : 2;
break;

case 0xA:
//ExA1 - SKNP Vx
Vx = n2;
PC += !keypad[V[Vx]] ? 4 : 2;
break;
}
break;

case 0xF:
switch (n3) {
case 0x0:
switch (n4) {
case 0x7:
//Fx07 - LD Vx, DT
Vx = n2;
V[Vx] = DT;
PC += 0x2;
break;

case 0xA:
//Fx0A - LD Vx, K
Vx = n2;
for (i = 0; i < 16; i++) if (keypad[i]) { key = i; break; }
if (keypad[i] == 0 && key <= 0xF) {
V[Vx] = key;
key = 0x10;
PC += 0x2;
}
break;
}
break;

case 0x1:
switch (n4) {
case 0x5:
//Fx15 - LD DT, Vx
Vx = n2;
DT = V[Vx];
PC += 0x2;
break;

case 0x8:
//Fx18 - LD ST, Vx
Vx = n2;
ST = V[Vx];
PC += 0x2;
break;

case 0xE:
//Fx1E - ADD I, Vx
Vx = n2;
I += V[Vx];
PC += 0x2;
break;
}
break;

case 0x2:
//Fx29 - LD F, Vx
Vx = n2;
I = V[Vx] * 0x5;
PC += 0x2;
break;

case 0x3:
//Fx33 - LD B, Vx
Vx = n2;
h = V[Vx] / 100;
t = (V[Vx] - h * 100) / 10;
o = V[Vx] - h * 100 - t * 10;
mem[I] = h;
mem[I + 1] = t;
mem[I + 2] = o;
PC += 0x2;
break;

case 0x5:
//Fx55 - LD [I], Vx
Vx = n2;
for (i = 0; i <= Vx; i++) mem[I + i] = V[i];
I += (Vx + 1);
PC += 0x2;
break;

case 0x6:
//Fx65 - LD Vx, [I]
Vx = n2;
for (i = 0; i <= Vx; i++) V[i] = mem[I + i];
I += (Vx + 1);
PC += 0x2;
break;
}
break;
}
}
}




