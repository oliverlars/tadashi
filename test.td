fn function_one() {
	x = 20;
	return x + 1;
}

fn function_two() {
	return 1 + 2 + 3;
}

fn entry() {
	a = 1;
	if a < 2 {
		for 0 to 10 {
			a = a + 1;
		}
		a = a + function_one();
	}else {
		a = function_two();
	}
}

//add support for comments (like this one)
// and also strings (syntactic sugar around arrays)
// add unary - and +
//parse all functions (currently is hard coded to the above 3)
// while loops
// add divides!!!!!
// output debug info like where each block starts/ends (functions, ifs, fors, etc)

// 21 < 6
// 21 - 6

a = 6 < 21 // true is negative?

//0 sub RA, RB
//1 move RA, 0
//2 jumpp 4
//3 move RA, 1
//4 next instructions