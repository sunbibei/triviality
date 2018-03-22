#!/bin/bash
# define the vairables.
PWD=`pwd`
BUILD_WS=${PWD}/src/build
EXPER_PREFIX=${PWD}/experiment
EXPER_NUM=3
SCHEDULERS="FcfsScheduler FeedbackRRScheduler IdealSJFScheduler SJFScheduler RRScheduler"

# Time
SH_START=`date +%s%N`

###############################################################################################
######## COMPILE
###############################################################################################
echo "CLEAR THE OLD DATA OF EXPERIMENT"
# rm -rf $EXPER_WS
for num in {1..3}
do
	rm -rf ${EXPER_PREFIX}${num}
done

rm -rf ${BUILD_WS}

echo "COMPLIE THE ALL OF CODE"
mkdir ${BUILD_WS}

javac ./src/AbstractScheduler.java ./src/InputGenerator.java       ./src/BurstProcess.java\
	  ./src/InputReader.java       ./src/Event.java                ./src/Process.java\
	  ./src/ProcessModel.java      ./src/ExponentialGenerator.java ./src/RRScheduler.java\
	  ./src/FcfsScheduler.java     ./src/Simulator.java            ./src/FeedbackRRScheduler.java\
	  ./src/SJFScheduler.java      ./src/IdealSJFScheduler.java    ./src/EventProcessor.java -d ${BUILD_WS}

# javac AbstractScheduler.java InputGenerator.java BurstProcess.java\
# 	  InputReader.java Event.java Process.java EventProcessor.java\
# 	  ProcessModel.java ExponentialGenerator.java RRScheduler.java\
# 	  FcfsScheduler.java Simulator.java FeedbackRRScheduler.java\
# 	  SJFScheduler.java IdealSJFScheduler.java -d $BUILD_WS

cd ${BUILD_WS}
echo "Complie DONE"

###############################################################################################
######## EXPERIEMNT 1
###############################################################################################
EXPER_IDX=1
EXPER_DIR=${EXPER_PREFIX}${EXPER_IDX}
mkdir $EXPER_DIR
# cd    $EXPER_DIR
echo ""
echo ""
echo "EXPERIMENT 1 READY TO START..."
sleep 2s
EXPER_START=`date +%s%N`

I_DIR=${EXPER_DIR}/Input
mkdir -p ${I_DIR}

O_DIR=${EXPER_DIR}/Output
mkdir -p ${O_DIR}

####################################################
######## CREATE THE PARAMETERS OF SIMULATIONS
####################################################
for sch in ${SCHEDULERS}; do
	if [[ ${sch} == "RRScheduler" || ${sch} == "FeedbackRRScheduler" ]]; then
		for itr in {1..10}; do
			S_P_N=${I_DIR}/sim_${sch:0:2}_${itr}.prp
			touch ${S_P_N}
			echo "scheduler="${sch}         >> ${S_P_N}
			echo "timeLimit=10000"          >> ${S_P_N}
			echo "periodic=false"           >> ${S_P_N}
			echo "interruptTime=0"          >> ${S_P_N}
			echo "timeQuantum="$[${itr}*20] >> ${S_P_N}
			echo "initialBurstEstimate=10"  >> ${S_P_N}
			echo "alphaBurstEstimate=0.5"   >> ${S_P_N}
		done
	elif [ ${sch} == "SJFScheduler" ]; then
		for tau in {1..5}; do
			for alpha in {0..3}; do
				S_P_N=${I_DIR}/sim_${sch:0:2}_${tau}_${alpha}.prp
				touch $S_P_N
				echo "scheduler="${sch}                                      >> ${S_P_N}
				echo "timeLimit=10000"                                       >> ${S_P_N}
				echo "periodic=false"                                        >> ${S_P_N}
				echo "interruptTime=0"                                       >> ${S_P_N}
				echo "timeQuantum=20"                                        >> ${S_P_N}
				echo "initialBurstEstimate="$[${tau}*20]                     >> ${S_P_N}
				echo "alphaBurstEstimate=0"$(echo "${alpha}*0.2 + 0.3" | bc) >> ${S_P_N}
			done
		done
	else
		S_P_N=${I_DIR}/sim_${sch:0:2}.prp
		touch $S_P_N
		echo "scheduler="${sch}        >> ${S_P_N}
		echo "timeLimit=10000"         >> ${S_P_N}
		echo "periodic=false"          >> ${S_P_N}
		echo "interruptTime=0"         >> ${S_P_N}
		echo "timeQuantum=20"          >> ${S_P_N}
		echo "initialBurstEstimate=10" >> ${S_P_N}
		echo "alphaBurstEstimate=0.5"  >> ${S_P_N}
	fi
done

####################################################
######## RUN THE SIMULATION
####################################################
for idx in {1..20}; do
	I_P_N=${I_DIR}/inputs_${idx}.prp
	touch ${I_P_N}
	echo "seed="`date +%s`                >> ${I_P_N}
	echo "meanInterArrival=50.0"          >> ${I_P_N}
	echo "meanCpuBurst=15.0"              >> ${I_P_N}
	echo "meanIOBurst=15.0"               >> ${I_P_N}
	echo "meanNumberBursts=5.0"           >> ${I_P_N}
	echo "numberOfProcesses="$[${idx}*5]  >> ${I_P_N}
	echo "staticPriority=0"               >> ${I_P_N}

	# Generate the Input data
	java InputGenerator ${I_P_N} ${I_DIR}/inputs_${idx}.in

	# run simulation.
	for sch in ${SCHEDULERS}; do
		echo ""
		echo ""
		echo "==============================="
		echo ${sch}
		echo "==============================="
		if [[ ${sch} == "RRScheduler" || ${sch} == "FeedbackRRScheduler" ]]; then
			for itr in {1..10}; do
				S_P_N=${I_DIR}/sim_${sch:0:2}_${itr}.prp
				java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${itr}_${idx}.out ${I_DIR}/inputs_${idx}.in
			done
		elif [ ${sch} == "SJFScheduler" ]; then
			for tau in {1..5}; do
				for alpha in {0..3}; do
					S_P_N=${I_DIR}/sim_${sch:0:2}_${tau}_${alpha}.prp
					java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${tau}_${alpha}_${idx}.out ${I_DIR}/inputs_${idx}.in
				done
			done
		else
			S_P_N=${I_DIR}/sim_${sch:0:2}.prp
			java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${idx}.out ${I_DIR}/inputs_${idx}.in
		fi

		echo ${sch} "DONE"
		echo "-------------------------------"
	done
done

EXPER_END=`date +%s%N`
USE_TIME=`echo $EXPER_END $EXPER_START | awk '{ print ($1 - $2) / 1000000000}'`
echo "EXPERIMENT 1 DONE!!!" "LAPSE" ${USE_TIME}"s"
# exit

###############################################################################################
######## EXPERIEMNT 2
###############################################################################################
EXPER_IDX=2
EXPER_DIR=${EXPER_PREFIX}${EXPER_IDX}
mkdir $EXPER_DIR
# cd    $EXPER_DIR
echo ""
echo ""
echo "EXPERIMENT 2 READY TO START..."
sleep 2s
EXPER_START=`date +%s%N`

I_DIR=${EXPER_DIR}/Input
mkdir -p ${I_DIR}

O_DIR=${EXPER_DIR}/Output
mkdir -p ${O_DIR}

####################################################
######## CREATE THE PARAMETERS OF SIMULATIONS
####################################################
for sch in ${SCHEDULERS}; do
	if [[ ${sch} == "RRScheduler" || ${sch} == "FeedbackRRScheduler" ]]; then
		for itr in {1..10}; do
			S_P_N=${I_DIR}/sim_${sch:0:2}_${itr}.prp
			touch ${S_P_N}
			echo "scheduler="${sch}         >> ${S_P_N}
			echo "timeLimit=10000"          >> ${S_P_N}
			echo "periodic=false"           >> ${S_P_N}
			echo "interruptTime=0"          >> ${S_P_N}
			echo "timeQuantum="$[${itr}*20] >> ${S_P_N}
			echo "initialBurstEstimate=10"  >> ${S_P_N}
			echo "alphaBurstEstimate=0.5"   >> ${S_P_N}
		done
	elif [ ${sch} == "SJFScheduler" ]; then
		for tau in {1..5}; do
			for alpha in {0..3}; do
				S_P_N=${I_DIR}/sim_${sch:0:2}_${tau}_${alpha}.prp
				touch $S_P_N
				echo "scheduler="${sch}                                      >> ${S_P_N}
				echo "timeLimit=10000"                                       >> ${S_P_N}
				echo "periodic=false"                                        >> ${S_P_N}
				echo "interruptTime=0"                                       >> ${S_P_N}
				echo "timeQuantum=20"                                        >> ${S_P_N}
				echo "initialBurstEstimate="$[${tau}*20]                     >> ${S_P_N}
				echo "alphaBurstEstimate=0"$(echo "${alpha}*0.2 + 0.3" | bc) >> ${S_P_N}
			done
		done
	else
		S_P_N=${I_DIR}/sim_${sch:0:2}.prp
		touch $S_P_N
		echo "scheduler="${sch}        >> ${S_P_N}
		echo "timeLimit=10000"         >> ${S_P_N}
		echo "periodic=false"          >> ${S_P_N}
		echo "interruptTime=0"         >> ${S_P_N}
		echo "timeQuantum=20"          >> ${S_P_N}
		echo "initialBurstEstimate=10" >> ${S_P_N}
		echo "alphaBurstEstimate=0.5"  >> ${S_P_N}
	fi
done

####################################################
######## RUN THE SIMULATION
####################################################
for idx in {1..20}; do
	I_P_N=${I_DIR}/inputs_${idx}.prp
	touch ${I_P_N}
	echo "seed="`date +%s`         >> ${I_P_N}
	echo "meanInterArrival=50.0"   >> ${I_P_N}
	echo "meanCpuBurst=20"         >> ${I_P_N}
	echo "meanIOBurst="$[${idx}*5] >> ${I_P_N}
	echo "meanNumberBursts=5.0"    >> ${I_P_N}
	echo "numberOfProcesses=60"    >> ${I_P_N}
	echo "staticPriority=0"        >> ${I_P_N}

	# Generate the Input data
	java InputGenerator ${I_P_N} ${I_DIR}/inputs_${idx}.in

	# run simulation.
	for sch in ${SCHEDULERS}; do
		echo ""
		echo ""
		echo "==============================="
		echo ${sch}
		echo "==============================="
		if [[ ${sch} == "RRScheduler" || ${sch} == "FeedbackRRScheduler" ]]; then
			for itr in {1..10}; do
				S_P_N=${I_DIR}/sim_${sch:0:2}_${itr}.prp
				java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${itr}_${idx}.out ${I_DIR}/inputs_${idx}.in
			done
		elif [ ${sch} == "SJFScheduler" ]; then
			for tau in {1..5}; do
				for alpha in {0..3}; do
					S_P_N=${I_DIR}/sim_${sch:0:2}_${tau}_${alpha}.prp
					java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${tau}_${alpha}_${idx}.out ${I_DIR}/inputs_${idx}.in
				done
			done
		else
			S_P_N=${I_DIR}/sim_${sch:0:2}.prp
			java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${idx}.out ${I_DIR}/inputs_${idx}.in
		fi

		echo ${sch} "DONE"
		echo "-------------------------------"
	done
done

EXPER_END=`date +%s%N`
USE_TIME=`echo $EXPER_END $EXPER_START | awk '{ print ($1 - $2) / 1000000000}'`
echo "EXPERIMENT 2 DONE!!!" "LAPSE" ${USE_TIME}"s"

###############################################################################################
######## EXPERIEMNT 3
###############################################################################################
EXPER_IDX=3
EXPER_DIR=${EXPER_PREFIX}${EXPER_IDX}
mkdir $EXPER_DIR
# cd    $EXPER_DIR
echo ""
echo ""
echo "EXPERIMENT 3 READY TO START..."
sleep 2s
EXPER_START=`date +%s%N`

I_DIR=${EXPER_DIR}/Input
mkdir -p ${I_DIR}

O_DIR=${EXPER_DIR}/Output
mkdir -p ${O_DIR}

####################################################
######## CREATE THE PARAMETERS OF SIMULATIONS
####################################################
for sch in ${SCHEDULERS}; do
	if [[ ${sch} == "RRScheduler" || ${sch} == "FeedbackRRScheduler" ]]; then
		for itr in {1..10}; do
			S_P_N=${I_DIR}/sim_${sch:0:2}_${itr}.prp
			touch ${S_P_N}
			echo "scheduler="${sch}         >> ${S_P_N}
			echo "timeLimit=10000"          >> ${S_P_N}
			echo "periodic=false"           >> ${S_P_N}
			echo "interruptTime=0"          >> ${S_P_N}
			echo "timeQuantum="$[${itr}*20] >> ${S_P_N}
			echo "initialBurstEstimate=10"  >> ${S_P_N}
			echo "alphaBurstEstimate=0.5"   >> ${S_P_N}
		done
	elif [ ${sch} == "SJFScheduler" ]; then
		for tau in {1..5}; do
			for alpha in {0..3}; do
				S_P_N=${I_DIR}/sim_${sch:0:2}_${tau}_${alpha}.prp
				touch $S_P_N
				echo "scheduler="${sch}                                      >> ${S_P_N}
				echo "timeLimit=10000"                                       >> ${S_P_N}
				echo "periodic=false"                                        >> ${S_P_N}
				echo "interruptTime=0"                                       >> ${S_P_N}
				echo "timeQuantum=20"                                        >> ${S_P_N}
				echo "initialBurstEstimate="$[${tau}*20]                     >> ${S_P_N}
				echo "alphaBurstEstimate=0"$(echo "${alpha}*0.2 + 0.3" | bc) >> ${S_P_N}
			done
		done
	else
		S_P_N=${I_DIR}/sim_${sch:0:2}.prp
		touch $S_P_N
		echo "scheduler="${sch}        >> ${S_P_N}
		echo "timeLimit=10000"         >> ${S_P_N}
		echo "periodic=false"          >> ${S_P_N}
		echo "interruptTime=0"         >> ${S_P_N}
		echo "timeQuantum=20"          >> ${S_P_N}
		echo "initialBurstEstimate=10" >> ${S_P_N}
		echo "alphaBurstEstimate=0.5"  >> ${S_P_N}
	fi
done

####################################################
######## RUN THE SIMULATION
####################################################
for idx in {1..20}; do
	I_P_N=${I_DIR}/inputs_${idx}.prp
	touch ${I_P_N}
	echo "seed="`date +%s`          >> ${I_P_N}
	echo "meanInterArrival=50.0"    >> ${I_P_N}
	echo "meanCpuBurst="$[${idx}*5] >> ${I_P_N}
	echo "meanIOBurst=20"           >> ${I_P_N}
	echo "meanNumberBursts=5.0"     >> ${I_P_N}
	echo "numberOfProcesses=60"     >> ${I_P_N}
	echo "staticPriority=0"         >> ${I_P_N}

	# Generate the Input data
	java InputGenerator ${I_P_N} ${I_DIR}/inputs_${idx}.in

	# run simulation.
	for sch in ${SCHEDULERS}; do
		echo ""
		echo ""
		echo "==============================="
		echo ${sch}
		echo "==============================="
		if [[ ${sch} == "RRScheduler" || ${sch} == "FeedbackRRScheduler" ]]; then
			for itr in {1..10}; do
				S_P_N=${I_DIR}/sim_${sch:0:2}_${itr}.prp
				java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${itr}_${idx}.out ${I_DIR}/inputs_${idx}.in
			done
		elif [ ${sch} == "SJFScheduler" ]; then
			for tau in {1..5}; do
				for alpha in {0..3}; do
					S_P_N=${I_DIR}/sim_${sch:0:2}_${tau}_${alpha}.prp
					java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${tau}_${alpha}_${idx}.out ${I_DIR}/inputs_${idx}.in
				done
			done
		else
			S_P_N=${I_DIR}/sim_${sch:0:2}.prp
			java Simulator ${S_P_N} ${O_DIR}/output_${sch:0:2}_${idx}.out ${I_DIR}/inputs_${idx}.in
		fi

		echo ${sch} "DONE"
		echo "-------------------------------"
	done
done

EXPER_END=`date +%s%N`
USE_TIME=`echo $EXPER_END $EXPER_START | awk '{ print ($1 - $2) / 1000000000}'`
echo "EXPERIMENT 3 DONE!!!" "LAPSE" ${USE_TIME}"s"

SH_END=`date +%s%N`
USE_TIME=`echo $SH_END $SH_START | awk '{ print ($1 - $2) / 1000000000}'`
echo ""
echo ""
echo "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"
echo "EVERYTHING HAS DONE!" "LAPSE" ${USE_TIME}"s"
echo "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"