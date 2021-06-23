# $Id: split_Action.awk,v 1.1.1.1 2006-11-27 22:36:16 eugene Exp $

# Usage: gawk -f split_Action.awk xxxxxAction.c

# Script to turn xxxxAction.c into a set of .c and .h files in a directory.
# The format of the source is a comment block and function.
# Recognize the beginning of a header block and the end of the prev function.
# Recognize the prototype following the comment block.
# Dump the include and define lines in every file produced.
# We don't expect to find any of those lines after we start finding functions!

BEGIN {
        found = 0
        head_found = 0

        head = ""
	allincludes = ""
	alldefines = ""
	prototype = ""

	function_name = "n"
	function_head = "h"
	function_count = 0

	# catch-up queue of 2 lines
	last1line = ""
	last2line = ""

        # directory to put stuff
	dir = "x"
	mkdir -p dir
}

/#include/	{
	allincludes = allincludes "\n" $0
	#print "found an include!"
	#print $0
	#print ""
	#print "allincludes is " allincludes
	if ( found == 1 ) {
		print "include found after function found?" $NL
	}
	found = 0
	head_found = 0
	next;
}

/#define/	{
	alldefines = alldefines "\n" $0
	#print "found an define!"
	#print $0
	#print ""
	#print "alldefines is " alldefines
	if ( found == 1 ) {
		print "define found after function found?" $NL
	}
	found = 0
	head_found = 0
	next;
}

# comment block at the start of a function always begins this way
/Function: /	{
		function_count++

		function_name = $4
		# strip off the pesky leading star on the function name
		if ( $4 ~ /\*/ ) {
			function_name = substr($4,2)
		}

		# warm fuzzies
		print "processing " function_name

		head = function_name
		function_head = function_name".h"

		# put the c file in a directory
		function_name = dir"/"function_name".c"

		print alldefines > function_name
		print allincludes >> function_name

		# always reference our prototype header file
		print "#include \""function_head"\"" >> function_name
		print "\n" >> function_name

		# now we can put the header in a directory too
		function_head = dir"/"function_head

		# fake out the last two lines
		last2line = "\n"
		last1line = "/*" 
		found = 1
}

# match any line not recognized already
//	{
	if ( found == 1 ) {
		# finesse: skip duplicate lines
		if ( last2line != last1line ) {
			print last2line >> function_name
		}

		# play catch-up on the last two lines--if needed
		last2line = last1line
		last1line = $0

		# accumulate the prototype information
		if (head_found == 1 ) {
			prototype = prototype " " $0
		}

		if ( $2 ~ head ) {
			head_found = 1
			prototype = $0
		} 
		
		# write out the function prototype in a header file
		if (prototype ~ ")"  && head_found == 1 ) {

			print "#ifndef "head"_H__\n#define "head"_H__\n" > function_head
			print prototype";\n" >> function_head
			print "#endif /* "head"_H__ */\n" >> function_head

				head_found = 0
		}
	} 
}

END {
	# flush the catch-up queue if we ran out while processing a function
	if ( found == 1 ) {
			print $0 >> function_name
	}

	# _everybody_ loves stats
	print "functions processed: " function_count 
}


