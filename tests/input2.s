
.global d,m
.text
a:
.data
dataLabela: .char 1,2,3
.skip 10

d: 
.long 1500000,11
.align 4

e:
 .word 6,7

.rodata
s: .char 4,5


.bss
k: .skip 8
.align 3

.end

