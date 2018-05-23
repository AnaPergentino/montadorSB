load n
section text
teste: macro &a,&b,&c
load &a
input &b
input &c
endmacro 
load n
input n
load #0
input #1
input #2
load n
input n
input x+l2
section data
n: space 1
x: space 3
