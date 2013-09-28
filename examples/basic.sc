var x = 15;

# Expected bytecode:
# ALLOC 	G_1
# PUSH 		G_1
# ALLOC 	G_2
# MOV_I 	G_2, 15
# PUSH 		G_2 
# POPMOV 	G_1
# POP