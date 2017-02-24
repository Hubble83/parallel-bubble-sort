#############################################################################################
#																							#
#	Usage:																					#
#	[																						#
#		[executables],																		#
#		[optional variations],																#
#		...,																				#
#		{"curve label" :[curves]},															#
#		{"x label" :[curve points]},														#
#		"y label",																			#
#		[k, repetitions]																	#
#		["flags"]																			#
#	]																						#
#																							#
#############################################################################################

[
	[
		"sequential",
		"omp_odd_even",
		"omp_busy_wait",
		"omp_mutex",
		"pthreads_busy_wait",
		"pthreads_mutex"
	],
	[8192,65536],
	["g"],	
	{ "Chunk size = " : [128,512,2048] },
	{ "Number of threads" : range(2,49,2) },
	"Speedup",
	[3,8] #kbest
]

#############################################################################################
#																							#
#	Generates all combinations of elements from each list									#
#																							#
#############################################################################################
