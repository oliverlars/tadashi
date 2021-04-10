fn nonsense(imax, j){
  kk = imax * j;
  return kk; 
}


fn entry() {

    the_max = 10;
    j = 2;
    k = 0;

    for i = 0; i < the_max; i = i + 1 {

      k = k + nonsense(the_max, j);
    }
    return 0;
}