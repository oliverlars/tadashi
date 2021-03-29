fn test() {
	x = 20;
	return x + 1;
}

fn test_() {
	return 1 + 2 + 3;
}

fn entry() {
	a = [2];
	a[0] = test() + test_();
	x = 10;
	a[1] = x + 2;
	return x + 2;
}