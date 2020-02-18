../bonnmotion-3.0.1/bin/bm -f scenario2 RandomWaypoint -n 9 -d 1 -i 0 -x 200 -y 200
../bonnmotion-3.0.1/bin/bm IntervalFormat -f scenario2 -l 1
../BMARK_BASE_GENERATOR/generator.exe -o bm_9ue_4base_station_2s.dat -b 4 -u 9 -x 200 -y 200 -t 2 -T 2 -w scenario2.if

