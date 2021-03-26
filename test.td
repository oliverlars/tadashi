fn test() {
	return 1 + 2 + 3;
}

fn entry() {
	a = [10];
	index = 0;
	for 1 to 10 {
		a[index] = 1 + (index+1)*(index+1);
		index = index + 1;
	}
}