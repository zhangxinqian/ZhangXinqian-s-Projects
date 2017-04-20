//This file was generated from (Academic) UPPAAL 4.1.18 (rev. 5444), November 2013

/*

*/
Pr[<=100] (<>(visit[7]>0))

/*

*/
Pr[<=100](<>(n13.Done))

/*

*/
simulate 5 [<=100] {n7.clk}

/*

*/
Pr[<=100](<>(n2.Done))

/*

*/
Pr[g_time<=30](<>(n7.Done))

/*

*/
Pr[<=100](<>(n7.Done))
