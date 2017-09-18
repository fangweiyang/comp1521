 .data
msg1:
   .asciiz "#"
msg2:
   .asciiz "."
msg3:
   .asciiz "\n"
print1:
   .asciiz "=== After iteration "
print2:
   .asciiz "===\n"
print:
   .asciiz "# Iterations: "



   .align 2
main_ret_save:
   .space 4

   .text
   .globl main
main:
   sw   $ra, main_ret_save
   lw   $ra, main_ret_save

   #printf("# Iterations: ");
   li   $v0, 4
   la   $a0, print
   syscall

   #scanf("%d", &maxiters);
   li    $v0,5
   syscall
   move $t0,$v0   #t0 is max

#for (int n = 1; n <= maxiters; n++) {
li $t1,1   #s1 is n
main_for_loop1:
   bgt  $t1,$t0,end_main_for_loop1
   #  for (int i = 0; i < N; i++) {
    li $t2,0       #i is s2
    main_for_loop2:
    lw $s0,N
    bge $t2,$s0,end_main_for_loop2
#   li   $v0, 4
#   la   $a0, msg2
#    syscall

    li $t3,0        #j is s3
    #for (int j = 0; j < N; j++) {
    mian_for_loop3:
#    li $v0,1
#    move $a0,$t3
#    syscall

       lw $s0,N
       bge $t3,$s0,end_main_for_loop3
       move $a0,$t2
       move $a1,$t3
       jal neighbours
       nop
       move $t4,$v0      #nn is $s4
       lw $s0,N
       mul $s1,$s0,$t2
       add $s1,$s1,$t3
       lb   $t5  , board($t1)
       main_if_1:                         #if (board[i][j] == 1)
           li $s3,1
           bne $t4,$s3,else_main_if_1
           j main_if_2
           main_if_2:			 #if (nn < 2)
              li $s4,2
              bge $t4,$s4,main_else_if_2
              sb $0,newBoard($t1)
              addi $t3,$t3,1
              j mian_for_loop3
           main_else_if_2:
              li $s5,3
              li $s4,2
              beq $t4,$s5,main_state
              beq $t4,$s4,main_state
              j  main_else_2
          main_else_2:
             sb $0,newBoard($t1)
             addi $s3,$s3,1
             j mian_for_loop3
           main_state:
             li $t3,1
             sb $t3,newBoard($t1)
             addi $s3,$s3,1
             j  mian_for_loop3
       else_main_if_1:
          li $t5,3
          bne  $s4,$t5,else_main_1
          li $t3,1
          sb $t3,newBoard($t1)
          addi $t3,$t3,1
          j  mian_for_loop3
       else_main_1:
          sb $0,newBoard($t1)
          addi $t3,$t3,1
          j  mian_for_loop3
       end_main_for_loop3:
    addi $t2,$t2,1
    j main_for_loop2
    end_main_for_loop2:

   la   $a0, print1       # print1
   li   $v0, 4
   syscall

   move $a0,$t1
   li   $v0, 1
   syscall

   la   $a0, print2       # print2
   li   $v0, 4
   syscall

   jal copyBackAndShow
   nop
   addi $t1,$t1,1
   j main_for_loop1
end_main_for_loop1:
end_main:
   lw   $ra, main_ret_save
   jr   $ra





neighbours:
addi $sp, $sp, -4
sw   $fp, ($sp)
move $fp, $sp
addi $sp, $sp, -4
sw   $ra, ($sp)

 move $s0,$a0   #$s0=i
 move $s1,$a1   #$s1=j
 li   $s2,-1    #$s2=-1 $s2 is x
 li   $s3,-1    #$s3=-1 $s3 is y
 li   $s6, 0    #$s6 is nn=0
 li   $s4, 1
 lw   $s5, N     #$s5=N
 addi $s7,$s5,-1  #t2=N-1
for_loop1:
    bgt $s2,$s4,end_for_loop1        #x>1 break
    li  $s3,-1
    for_loop2:
       bgt $s3,$s4,end_for_loop2     #y>1 break
       if_1_start:			#if (i+x < 0 || i+x > N-1) continue;
            add $t1,$s0,$s2        #t1=x+i
            blt $t1,$0,endif       # x+i<0
            bgt  $t1,$s7,endif
        if_2_start:			#if (j+y < 0 || j+y > N-1) continue;
            add  $t3,$s1,$s3        #t3=j+y
            blt  $t3,$0,endif
            bgt  $t3,$s7,endif
        if_3_start:		# if (x == 0 && y == 0) continue;
            bne $s2,$0,if_4_start
            bne $s3,$0,if_4_start
            j endif
        if_4_start:			#if (board[i+x][j+y] == 1
            add $t1,$s0,$s2         #i+x
            add  $t3,$s1,$s3        #j+y
            mul $t4,$s5,$t1         #(x+i)N
            add $t4,$t4,$t3         #(X+i)N+(j+y)
            lb  $t5,board($t4)
            li  $t0,1                 #t0=1
            beq $t5,$t0,state_2
            j endif
        state_2:
            addi $s6,$s6,1            #nn++
            j endif
        endif:
            addi,$s3,$s3,1               #y++
            j for_loop2
    end_for_loop2:
    addi,$s2,$s2,1                    #x++
    j for_loop1
end_for_loop1:
move $v0,$s6
lw   $ra, ($sp)
addi $sp, $sp, 4
lw   $fp, ($sp)
addi $sp, $sp, 4
jr   $ra



copyBackAndShow:
   addi $sp, $sp, -4
   sw   $fp, ($sp)
   move $fp, $sp
   addi $sp, $sp, -4
   sw   $ra, ($sp)
   addi $sp, $sp, -200          # int board[100];

    li $t0,0
    lw $s0,N                    #N = s0
    li $s1,0 			#i = 0
loop1:
    beq $s1,$s0,end_loop1        #i>=N break
    li  $s2,0 			 #j = 0
    loop2:
       beq  $s2, $s0, end_loop2               #j>=N break
       mul  $t0, $s1, $s0		      #t0=i*N
       add  $t0, $t0, $s2		      #t0=t0+j,t0=i*N+j
       lb   $s3, newBoard($t0)
       sb   $s3, board($t0)
       if:
          beq $s3,$0,state_1               #if(board==0)
          bne $s3,$0,else
       state_1:
          la   $a0, msg2       #putchar('.');
          li   $v0, 4
          syscall
          j end_if
       else:
          la   $a0, msg1       #putchar('#');
          li   $v0, 4
          syscall
      end_if:
      addi $s2,$s2,1         #j++
      j loop2
   end_loop2:
   la   $a0, msg3       #putchar('\n');
   li   $v0, 4
   syscall

   addi $s1,$s1,1           #i++
   j loop1
end_loop1:
   addi $sp, $sp, 200
   lw   $ra, ($sp)
   addi $sp, $sp, 4
   lw   $fp, ($sp)
   addi $sp, $sp, 4
   jr   $ra
