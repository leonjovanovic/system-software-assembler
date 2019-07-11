


.global d,m


.text


a:


add r1,r1


addb r1,r2


addw r1,r2










 add r1,10


 sub r2,r3


 mul c,r1


 div *20,r1


 cmp r3[10],r3


 and r5[e],r1


 or r5[e],r1


c: 


 not r1





 push r2


 pop r2


 shl r4,r5


 shr *21,r1


 iret


 call c








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


.align 4





.end













