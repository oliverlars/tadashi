fn test() {
	return 1 + 2 + 3;
}

fn entry() {
	a = [10];
	a[0] = 50;
	a[1] = 20;
	potato = a[0] + a[1];
}

//RD doesnt use a stack so i cannot nest function calls
//mimic call/ret using jumpu on RC (function call register)
//every 'call' will also increment RC pointer and store address at that new location
//reserve stack space for it somewhere