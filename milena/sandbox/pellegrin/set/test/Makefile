all:
	./test.sh
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_bgraph.cc -o bgraph
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_priority_queue.cc -o priority_queue
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_priority_queue_fast.cc -o priority_queue_fast
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_priority_queue_fast_with_array.cc -o priority_queue_fast_with_array
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_queue.cc -o queue
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_queue_fast.cc -o queue_fast
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_runs.cc -o runs
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ p_set.cc -o set
	g++-4.2 -ansi -pedantic -W -Wall -Werror -Wextra -I../../../.. -I../ pset_if.cc -o pset_if
	./detest.sh
	
clean:
	rm -vf bgraph priority_queue priority_queue_fast priority_queue_fast_with_array \
	  queue queue_fast runs set pset_if
