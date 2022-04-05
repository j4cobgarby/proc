0	M[PC] -> B; PC++	# B is prev 1		010c
1	1						0001
2	M[PC] -> C; PC++	# C is prev 2		020c
3	1						0001
4	B + C -> A		# A is current value	2103
5	B -> C						210a
6	A -> B						100a
7	M[PC] -> PC					000b
8	4						0004
