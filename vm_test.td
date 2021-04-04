


/*fn entry() {
	x = 20;
	return x + 3;
}*/
fn emit_jump_unconditional(address) {
	a = 32 << (24-6);
	a = a + address;
	return a;
}

fn entry() {
	memory = [101];
	memory[31] = emit_jump_unconditional(0);
	memory[32] = 9961506;

	for i = 0; i < 10; i = i + 1 {
		if i == 1 {
			memory[3] = 15;
		}
	}
	return memory[3];
}