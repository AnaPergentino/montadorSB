load n
section text
teste: macro &a,&b,&c
load &a
input &b
input &c
endmacro 
load n
input n
load &a
input &a
input &a
load n
input n
input x+l2
section data
n: space 1
x: space 3
