python create_tournament.py
TOURNAMENT=tournament_2_5.in
../bin/quixxmaster -s 0 -m mem_0_2_5.qxm < $TOURNAMENT > res0_2_5.txt
../bin/quixxmaster -s 100 -m mem_100_2_5.qxm < $TOURNAMENT > res100_2_5.txt
../bin/quixxmaster -s 10 < $TOURNAMENT > res10_2_5.txt
../bin/quixxmaster -s 2 < $TOURNAMENT > res2_2_5.txt
python evaluate_tournament.py
