fn test() {
	return 1 + 2 + 3;
}

fn entry() {
	a = [2];
	a[0] = test();
}

//RD doesnt use a stack so i cannot nest function calls
//mimic call/ret using jumpu on RC (function call register)
//every 'call' will also increment RC pointer and store address at that new location
//reserve stack space for it somewhere (aka addresses 512 and up or something)

on stack set RC to 512

RC stores stack pointer

call: RD <- pc
call2: increment RC