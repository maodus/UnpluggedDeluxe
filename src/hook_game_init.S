.set push
.set noreorder 

.text
.global h_game_init
.ent h_game_init



h_game_init:
  addiu $sp, $sp, -16
  sw $ra, 0($sp)
  sw $a0, 4($sp)
  sw $fp, 8($sp)

  jal inject_custom
  sw $t9, 12($sp)
        
  move $a1, $v0
  lw $ra, 0($sp)  
  lw $a0, 4($sp)
  lw $fp, 8($sp)
  lw $t9, 12($sp) 

  jr $t9  
  addiu $sp, $sp, 16
.end h_game_init

.set pop