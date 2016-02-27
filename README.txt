Input file must have the name 'inp-params.txt'

To run and check the average time to enter CS of Filter Lock algorithm:
$ g++ -pthread filterLock.cpp -std=gnu++11
$ ./a.out && python mergefiles.py && python analyse.py

To run and check the average time to enter CS of Bakery algorithm:
$ g++ -pthread bakery.cpp -std=gnu++11
$ ./a.out && python mergefiles.py && python analyse.py