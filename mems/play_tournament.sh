python create_tournament.py
TOURNAMENT=tournament.in
../bin/qwixxmaster -s 0 -m mem_0.qxm < $TOURNAMENT > res_0.txt
../bin/qwixxmaster -s 1000 -m mem_1000.qxm < $TOURNAMENT > res_1000.txt
#../bin/qwixxmaster -s 100 -m mem_100.qxm < $TOURNAMENT > res_100.txt
#../bin/qwixxmaster -s 10 -m mem_10.qxm < $TOURNAMENT > res_10.txt
#../bin/qwixxmaster -s 2 < $TOURNAMENT > res_2.txt
python evaluate_tournament.py
