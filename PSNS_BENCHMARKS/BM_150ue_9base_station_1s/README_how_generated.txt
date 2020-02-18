../bonnmotion-3.0.1/bin/bm -f scenario2 RandomWaypoint -n 150 -d 999 -i 0 -x 200 -y 200
../bonnmotion-3.0.1/bin/bm IntervalFormat -f scenario2 -l 1
../BMARK_BASE_GENERATOR/generator.exe -o bm_150ue_9base_station_2s.dat -b 9 -u 150 -x 200 -y 200 -t 1000 -T 1000 -w scenario2.if

