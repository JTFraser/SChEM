 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelM_All -mode 1 -dt 0.2 -constrain m -allfrac 1 -gradient 0
 sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelM_Cold -mode 1 -dt 0.2 -constrain m -allfrac 0 -gradient 0
  sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelL_All -mode 1 -dt 0.2 -constrain l -allfrac 1 -gradient 0
  sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelL_Cold -mode 1 -dt 0.2 -constrain l -allfrac 0 -gradient 0
  sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelT_All -mode 1 -dt 0.2 -constrain t -allfrac 1 -gradient 0
 sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelT_Cold -mode 1 -dt 0.2 -constrain t -allfrac 0 -gradient 0
 sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelS_All -mode 1 -dt 0.2 -constrain s -allfrac 1 -gradient 0
  sleep 1
 ./submitShort -threads 16 -random 10000000 -grid 101 -dir Output/ModelS_Cold -mode 1 -dt 0.2 -constrain s -allfrac 0 -gradient 0
  sleep 1.
 /submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelM_All_Grad -mode 1 -dt 0.2 -constrain m -allfrac 1 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelM_Cold_Grad -mode 1 -dt 0.2 -constrain m -allfrac 0 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelL_All_Grad -mode 1 -dt 0.2 -constrain l -allfrac 1 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelL_Cold_Grad -mode 1 -dt 0.2 -constrain l -allfrac 0 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelT_All_Grad -mode 1 -dt 0.2 -constrain t -allfrac 1 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelT_Cold_Grad -mode 1 -dt 0.2 -constrain t -allfrac 0 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelS_All_Grad -mode 1 -dt 0.2 -constrain s -allfrac 1 -gradient 1
  sleep 1
 ./submitLong -threads 28 -random 10000000 -grid 101 -dir Output/ModelS_Cold_Grad -mode 1 -dt 0.2 -constrain s -allfrac 0 -gradient 1
