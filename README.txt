To compile the code, just type make in the source folder:
$ make
gcc -Wall -g -c license.c
gcc -Wall -g -c common.c
gcc -Wall -g -o runsim runsim.c license.o common.o
gcc -Wall -g -o testsim testsim.c license.o common.o

To run the code, I used the example data file:
$ ./runsim 5 < testing.data
P231703| License file is /tmp/license.1000
P231703: 5 license added
P231703: Using license 4
P231703: Launching child on license 4
P231703: Using license 3
P231703: Launching child on license 3
P231703: Using license 2
P231703: Launching child on license 2
P231703: Using license 1
P231703: Launching child on license 1
P231703: Using license 0
P231703: Launching child on license 0
P231711: logging -> 10/05/21 14:07:44	231711	0 12
P231711: logging -> 10/05/21 14:07:45	231711	1 12
P231711: logging -> 10/05/21 14:07:46	231711	2 12
P231713: logging -> 10/05/21 14:07:47	231713	0 10
P231711: logging -> 10/05/21 14:07:47	231711	3 12
P231707: logging -> 10/05/21 14:07:48	231707	0 10
P231711: logging -> 10/05/21 14:07:48	231711	4 12
P231711: logging -> 10/05/21 14:07:49	231711	5 12
P231711: logging -> 10/05/21 14:07:50	231711	6 12
P231708: logging -> 10/05/21 14:07:51	231708	0 10
P231713: logging -> 10/05/21 14:07:51	231713	1 10
P231711: logging -> 10/05/21 14:07:51	231711	7 12
P231711: logging -> 10/05/21 14:07:52	231711	8 12
P231707: logging -> 10/05/21 14:07:53	231707	1 10
P231711: logging -> 10/05/21 14:07:53	231711	9 12
P231711: logging -> 10/05/21 14:07:54	231711	10 12
P231713: logging -> 10/05/21 14:07:55	231713	2 10
P231711: logging -> 10/05/21 14:07:55	231711	11 12
P231703: license returned
P231703: 1/5 processes exited
P231712: logging -> 10/05/21 14:07:56	231712	0 6
P231707: logging -> 10/05/21 14:07:58	231707	2 10
P231708: logging -> 10/05/21 14:07:59	231708	1 10
P231713: logging -> 10/05/21 14:07:59	231713	3 10
P231707: logging -> 10/05/21 14:08:03	231707	3 10
P231713: logging -> 10/05/21 14:08:03	231713	4 10
P231708: logging -> 10/05/21 14:08:07	231708	2 10
P231713: logging -> 10/05/21 14:08:07	231713	5 10
P231707: logging -> 10/05/21 14:08:08	231707	4 10
P231712: logging -> 10/05/21 14:08:09	231712	1 6
P231713: logging -> 10/05/21 14:08:11	231713	6 10
P231707: logging -> 10/05/21 14:08:13	231707	5 10
P231708: logging -> 10/05/21 14:08:15	231708	3 10
P231713: logging -> 10/05/21 14:08:15	231713	7 10
P231707: logging -> 10/05/21 14:08:18	231707	6 10
P231713: logging -> 10/05/21 14:08:19	231713	8 10
P231712: logging -> 10/05/21 14:08:22	231712	2 6
P231708: logging -> 10/05/21 14:08:23	231708	4 10
P231707: logging -> 10/05/21 14:08:23	231707	7 10
P231713: logging -> 10/05/21 14:08:23	231713	9 10
P231703: license returned
P231703: 2/5 processes exited
P231707: logging -> 10/05/21 14:08:28	231707	8 10
P231708: logging -> 10/05/21 14:08:31	231708	5 10
P231707: logging -> 10/05/21 14:08:33	231707	9 10
P231703: license returned
P231703: 3/5 processes exited
P231712: logging -> 10/05/21 14:08:35	231712	3 6
P231708: logging -> 10/05/21 14:08:39	231708	6 10
P231708: logging -> 10/05/21 14:08:47	231708	7 10
P231712: logging -> 10/05/21 14:08:48	231712	4 6
P231708: logging -> 10/05/21 14:08:55	231708	8 10
P231712: logging -> 10/05/21 14:09:01	231712	5 6
P231703: license returned
P231703: 4/5 processes exited
P231708: logging -> 10/05/21 14:09:03	231708	9 10
P231703: license returned
P231703: 5/5 processes exited
P231703: logging -> 10/05/21 14:09:03	231703	runsim terminated
P231703| Destroying license file /tmp/license.1000
