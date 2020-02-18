
Creates the file - n actors, d seconds
/bonnmotion-3.0.1/bin/bm -f scenario2 RandomWaypoint -n 9 -d 2 -i 0 -x 200 -y 200
Converts into interval of 1 s format
/bonnmotion-3.0.1/bin/bm IntervalFormat -f scenario2 -l 1

Once the above file is created then create benchmark b=base stations, u=UEs, t=timesteps in randomwaypoint, T=how many time steps to create in way point -w waypoint file
BMARK_BASE_GENERATOR/generator.exe -o bm_150ue_9base_station_2s.dat -b 9 -u 150 -x 200 -y 200 -t 2 -T 1 -w scenario2.if
