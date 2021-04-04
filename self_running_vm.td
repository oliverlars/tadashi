fn strlen(s){
	length = 0;
	while s[length] {
		length = length + 1;
	}
	return length;
}

fn emit_move_absolute(r, value) {
	a = 0;
	a = a + r << (24-6-2);
	a = a + value;
	return a;
}

fn emit_add_absolute(r, value) {
	a = 1;
	a = a + r << (24-6-2);
	a = a + value;
	return a;
}

fn emit_sub_absolute(r, value) {
	a = 2;
	a = a + r << (24-6-2);
	a = a + value;
	return a;
}

fn emit_load_absolute(r, address) {
	a = 3;
	a = a + r << (24-6-2);
	a = a + address;
	return a;
}

fn emit_store_absolute(r, address) {
	a = 4;
	a = a + r << (24-6-2);
	a = a + address;
	return a;
}

fn emit_and_absolute(r, value) {
	a = 5;
	a = a + r << (24-6-2);
	a = a + value;
	return a;
}

fn emit_or_absolute(r, value) {
	a = 6;
	a = a + r << (24-6-2);
	a = a + value;
	return a;
}

fn emit_xor_absolute(r, value) {
	a = 7;
	a = a + r << (24-6-2);
	a = a + value;
	return a;
}

fn emit_move_register(ra, rb) {
	a = 16 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_add_register(ra, rb) {
	a = 17 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_sub_register(ra, rb) {
	a = 18 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_load_register(ra, rb) {
	a = 19 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_store_register(ra, rb) {
	a = 20 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}


fn emit_and_register(ra, rb) {
	a = 21 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_or_register(ra, rb) {
	a = 22 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_xor_register(ra, rb) {
	a = 23 << (24-6);
	a = a + ra << (24-6-2);
	a = a + rb << (24-6-4);
	return a;
}

fn emit_sr_register(rx) {
	a = 24 << (24-6);
	a = a + rx << (24-6-2);
	return a;
}


fn emit_sl_register(rx) {
	a = 25 << (24-6);
	a = a + rx << (24-6-2);
	return a;
}

fn emit_jump_unconditional(address) {
	a = 32 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_zero(address) {
	a = 33 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_not_zero(address) {
	a = 34 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_carry(address) {
	a = 35 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_no_carry(address) {
	a = 36 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_negative(address) {
	a = 37 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_not_negative(address) {
	a = 38 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_positive(address) {
	a = 39 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_not_positive(address) {
	a = 40 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_overflow(address) {
	a = 41 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_no_overflow(address) {
	a = 42 << (24-6);
	a = a + address;
	return a;
}

fn emit_jump_register(address) {
	a = 43 << (24-6);
	a = a + address;
	return a;
}


fn get_opcode(opcode) {
	return (opcode >> 18) & 63;
}

fn get_register_x(inst) {
	return (inst >> 16) & 3;
}

fn get_register_y(inst) {
	return (inst >> 14) & 3;
}

fn get_address(inst) {
	return (inst & 65535);
}

fn get_operand(ins) {
	return (ins & 65535);
}

fn entry() {
	return 0;
	pc = 95;
	zero = 0;
	positive = 0;
	negative = 0;
	carry = 0;
	overflow = 0;
	rx = 0;
	ry = 0;
	registers = [4];
	registers[0] = 0;
	registers[1] = 0;
	registers[2] = 0;
	registers[3] = 0;
	
	memory = [4096];
	memory[0] = 8193;
	memory[1] = 1056768;
	memory[2] = 0;
	memory[3] = 1056779;
	memory[4] = 3;
	memory[5] = 1056780;
	memory[6] = 794636;
	memory[7] = 1056779;
	memory[8] = 794635;
	memory[9] = 270336;
	memory[10] = 1056779;
	memory[11] = 1; 
	memory[12] = 1056781;
	memory[13] = 794635;
	memory[14] = 262145;
	memory[15] = 860173;
	memory[16] = 5308416;
	memory[17] = 0;
	memory[18] = 1056782;
	memory[19] = 0;
	memory[20] = 1056783;
	memory[21] = 794639;
	memory[22] = 1056782;
	memory[23] = 20;
	memory[24] = 1056784;
	memory[25] = 794638;
	memory[26] = 1056785;
	memory[27] = 794641;
	memory[28] = 860176;
	memory[29] = 4734976;
	memory[30] = 1056785;
	memory[31] = 0;
	memory[32] = 9961506;
	memory[33] = 1;
	memory[34] = 1056785;
	memory[35] = 262144;
	memory[36] = 8650829;
	memory[37] = 15;
	memory[38] = 1056786;
	memory[39] = 794638;
	memory[40] = 1056787;
	memory[41] = 794643;
	memory[42] = 860178;
	memory[43] = 4734976;
	memory[44] = 1056787;
	memory[45] = 0;
	memory[46] = 8912944;
	memory[47] = 1;
	memory[48] = 1056787;
	memory[49] = 262144;
	memory[50] = 8650818;
	memory[51] = 0;
	memory[52] = 1056788;
	memory[53] = 3;
	memory[54] = 1056789;
	memory[55] = 794645;
	memory[56] = 1056788;
	memory[57] = 794644;
	memory[58] = 270336;
	memory[59] = 1056788;
	memory[60] = 15;
	memory[61] = 1056790;
	memory[62] = 794644;
	memory[63] = 262145;
	memory[64] = 860182;
	memory[65] = 5308416;
	memory[66] = 1;
	memory[67] = 1056791;
	memory[68] = 794638;
	memory[69] = 1056792;
	memory[70] = 794648;
	memory[71] = 860183;
	memory[72] = 4472832;
	memory[73] = 1056792;
	memory[74] = 794648;
	memory[75] = 1056782;
	memory[76] = 8388631;
	memory[77] = 0;
	memory[78] = 1056793;
	memory[79] = 3;
	memory[80] = 1056794;
	memory[81] = 794650;
	memory[82] = 1056793;
	memory[83] = 794649;
	memory[84] = 860160;
	memory[85] = 4472832;
	memory[86] = 1056793;
	memory[87] = 860185;
	memory[88] = 327680;
	memory[89] = 4997120;
	memory[90] = 1056795;
	memory[91] = 794651;
	memory[92] = 655361;
	memory[93] = 5210112;
	memory[94] = 11321344;
	memory[95] = 186072;
	memory[96] = 196709;
	memory[97] = 5472256;
	memory[98] = 393217;
	memory[99] = 196608;
	memory[100] = 11321344;

	if 0 {
		jumped = 0;
		if get_opcode(memory[pc]) == 0 {
			rx = get_register_x(memory[pc]);
			registers[rx] = get_operand(memory[pc]);
		}
		
		if get_opcode(memory[pc]) == 1 {
			rx = get_register_x(memory[pc]);
			registers[rx] = registers[rx] + get_operand(memory[pc]);
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 2 {
			rx = get_register_x(memory [pc]);
			registers[rx] = registers[rx] - get_operand(memory[pc]);
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 3 {
		
			rx = get_register_x(memory[pc]);
			address = get_address(memory[pc]);
			registers[rx] = memory[address];
		}
		
		if get_opcode(memory[pc]) == 4 {
			rx = get_register_x(memory[pc]);
			address = get_address(memory[pc]);
			memory[address] = registers[rx];
		}
		
		if get_opcode(memory[pc]) == 5 {
			rx = get_register_x(memory[pc]);
			operand = get_operand(memory[pc]);
			registers[rx] = registers[rx] & operand;
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 6 {
			rx = get_register_x(memory[pc]);
			operand = get_operand(memory[pc]);
			registers[rx] = registers[rx] | operand;
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 7 {
			rx = get_register_x(memory[pc]);
			operand = get_operand(memory[pc]);
			registers[rx] = registers[rx] & operand;
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 16 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			operand = get_operand(memory[pc]);
			registers[rx] = registers[ry];
		}
		
		if get_opcode(memory[pc]) == 17 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			registers[rx] = registers[rx] + registers[ry];
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 18 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			registers[rx] = registers[rx] - registers[ry];			
		}
		
		if get_opcode(memory[pc]) == 19 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			registers[rx] = memory[registers[ry]];			
		}
		
		if get_opcode(memory[pc]) == 20 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			memory[registers[ry]] = registers[rx];
		}
		
		if get_opcode(memory[pc]) == 21 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			registers[rx] = registers[rx] & registers[ry];
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 22 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			registers[rx] = registers[rx] | registers[ry];
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 23 {
			rx = get_register_x(memory[pc]);
			ry = get_register_y(memory[pc]);
			registers[rx] = registers[rx] ^ registers[ry];
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 24 {
			rx = get_register_x(memory[pc]);
			registers[rx] = registers[rx] >> 1;	
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 25 {
			rx = get_register_x(memory[pc]);
			registers[rx] = registers[rx] << 1;	
			zero = registers[rx] == 0;
			positive = registers[rx] > 0;
			negative = registers[rx] < 0;
		}
		
		if get_opcode(memory[pc]) == 26 {
		}
		
		if get_opcode(memory[pc]) == 27 {
		}
		
		if get_opcode(memory[pc]) == 32 {
			jumped = 1;
			address = get_address(memory[pc]);
			pc = address;
		}
		
		if get_opcode(memory[pc]) == 33 {
			if zero {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}
		}
		
		if get_opcode(memory[pc]) == 34 {
			if zero == 0 {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}
		}
		
		if get_opcode(memory[pc]) == 35 {
			if carry {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}
		}
		
		if get_opcode(memory[pc]) == 36 {
			if carry == 0 {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}
		
		if get_opcode(memory[pc]) == 37 {
			if negative {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}
		
		if get_opcode(memory[pc]) == 38 {
			if negative == 0 {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}
		
		if get_opcode(memory[pc]) == 39 {
			if positive {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}
		if get_opcode(memory[pc]) == 40 {
			if positive == 0 {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}
		
		if get_opcode(memory[pc]) == 41 {
			if overflow {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}
		
		if get_opcode(memory[pc]) == 41 {
			if overflow == 0 {
				jumped = 1;
				address = get_address(memory[pc]);
				pc = address;
			}	
		}		
		
		if get_opcode(memory[pc]) == 43 {
		}
		
		if jumped == 0 {
			pc = pc + 1;
		}
	}
}

// print out stack pointer location
// print out return address pointer location
// ebnf syntax


//add support for comments (like this one)
// add unary - and +
// while loops
// add divides!!!!!
// output debug info like where each block starts/ends (functions, ifs, fors, etc)
// equals
// and fix bugs
// try out making a strlen function (make strings c like)
//add true keyword

// a = [10]; make array of 10 numbers
// a == address of array
// a[0] first element of array
// pass by value to functions
// take a address and add offset
// dereference a + offset to read/write
// add string/array reassignment
// defo a scope bug (remove rx and ry from outer and it will say its undefined)
/*
	string = "zdsfjlasfjlk;";
	for x = 0; x < 5; x = x + 1 {
		random = random ^ (random << 13);
		random = random ^ (random >> 17);
		random = random ^ (random << 5);
		array[x] = random;
	}
	
	for pc = 0; pc < 13; pc = pc +1 {
		for j = pc + 1; j < 13; j = j + 1 {
			if string[pc] > string[j] {
				temp = string[pc];
				string[pc] = string[j];
				string[j] = temp;
			}
		}
	}
*/