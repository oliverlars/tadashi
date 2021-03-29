fn function_one() {
	x = 20;
	return x + 1;
}

fn function_two() {
	return 1 + 2 + 3;
}


fn entry() {
	string = "zdsfjlasfjlk;";
	for x = 0; x < 5; x = x + 1 {
		random = random ^ (random << 13);
		random = random ^ (random >> 17);
		random = random ^ (random << 5);
		array[x] = random;
	}
	
	for i = 0; i < 13; i = i +1 {
		for j = i + 1; j < 13; j = j + 1 {
			if string[i] > string[j] {
				temp = string[i];
				string[i] = string[j];
				string[j] = temp;
			}
		}
	}
}


//add support for comments (like this one)
// and also strings (syntactic sugar around arrays)
// add unary - and +
//parse all functions (currently is hard coded to the above 3)
// while loops
// add divides!!!!!
// output debug info like where each block starts/ends (functions, ifs, fors, etc)
// equals
// parameters
// and fix bugs
// try out making a strlen function
// separate declarations from assignments (to catch more errors)
/*
	string = "zdsfjlasfjlk;";
	for x = 0; x < 5; x = x + 1 {
		random = random ^ (random << 13);
		random = random ^ (random >> 17);
		random = random ^ (random << 5);
		array[x] = random;
	}
	
	for i = 0; i < 13; i = i +1 {
		for j = i + 1; j < 13; j = j + 1 {
			if string[i] > string[j] {
				temp = string[i];
				string[i] = string[j];
				string[j] = temp;
			}
		}
	}
*/