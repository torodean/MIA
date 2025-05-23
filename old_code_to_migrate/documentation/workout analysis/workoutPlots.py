#============================================================================
# Name        : workoutPlots.py
# Author      : Antonius Torode
# Date        : 3/28/18
# Copyright   : This file can be used under the conditions of Antonius' 
#				 General Purpose License (AGPL).
# Description : Program for generating workout plots for the documentation.
#============================================================================

import numpy as np
import matplotlib.pyplot as plt

#declare variables that are inputs or needed values for adjusting calculations.
toughness = 1
minNumOfExercises = 3.0
maxNumOfExercises = 25
minNumOfSets = 1.0
maxNumOfSets = 10.0
val = 1.9
weight = 1.0

def numMaxModifier(min, max, difficulty):
	'''
	Returns the modifier that determines the maximum of a thing allowed based on difficulty.
	'''
	return ( (max-min)/( 10.0**(4.0/3.0) ) )*difficulty**(2.0/3.0)+min
	
def numMaxModifierOLD(min, max, difficulty):
	'''
	Returns the modifier that determines the maximum of a thing allowed based on difficulty.
	'''
	return (max-min)/100.0 * difficulty + min
	
def numMinModifier(min, max, difficulty):
	'''
	Returns the modifier that determines the manimum of a thing allowed based on difficulty.
	'''
	return ( (max - min*val)/(10.0**(4.0/3.0)* val) )*difficulty**(2.0/3.0) + min
	
def numMinModifierOLD(min, max, difficulty):
	'''
	Returns the modifier that determines the manimum of a thing allowed based on difficulty.
	'''
	return (max-min)/200.0 * difficulty + min
	
	
def numAverage(min, max, difficulty):
	'''
	Inputs are minNumOfExercises and maxNumOfExercises and difficulty.
	Returns the average value of the first two inputs at the given difficulty.
	'''
	maxModifier = numMaxModifier(min, max, difficulty)
	minModifier = numMinModifier(min, max, difficulty)
	
	return (maxModifier+minModifier)/2.0
	
def numAverageOLD(min, max, difficulty):
	'''
	Inputs are minNumOfExercises and maxNumOfExercises and difficulty.
	Returns the average value of the first two inputs at the given difficulty.
	'''
	maxModifier = numMaxModifierOLD(min, max, difficulty)
	minModifier = numMinModifierOLD(min, max, difficulty)
	
	return (maxModifier+minModifier)/2.0
	
	
def numOfRepsMin(toughness, difficulty):
	'''
	Inputs are difficulty and toughness.
	Returns the min number of reps performed for a given difficulty.
	'''
	return (toughness*difficulty*weight)/2.0 + weight
	
def numOfRepsMax(toughness, difficulty):
	'''
	Inputs are difficulty and toughness.
	Returns the max number of reps performed for a given difficulty.
	'''
	return toughness*difficulty*weight+weight

def numOfRepsAverage(difficulty, toughness):
	'''
	Inputs are difficulty and toughness.
	Returns the average number of reps performed for a given difficulty.
	'''
	repsMin = numOfRepsMin(toughness, difficulty)
	repsMax = numOfRepsMax(toughness, difficulty)
	
	return (repsMax + repsMin)/2.0
	
	
def actualDifficulty(workouts, reps, sets):
	'''
	Inputs are workouts, reps, and sets.
	This function determines a relative actual difficulty based on inputs and returns a relative value.
	'''
	return workouts*reps*sets

#Create the difficulty array for plotting.
difficulty = np.arange(100)
	
#Set all of the average values for plotting.
aveExercises = numAverage(minNumOfExercises, maxNumOfExercises, difficulty)
aveSets = numAverage(minNumOfSets, maxNumOfSets, difficulty)
aveExercisesOLD = numAverageOLD(minNumOfExercises, maxNumOfExercises, difficulty)
aveSetsOLD = numAverageOLD(minNumOfSets, maxNumOfSets, difficulty)
aveReps = numOfRepsAverage(difficulty, toughness)

#Set all of the maximum values for plotting.
maxExercises = numMaxModifier(minNumOfExercises, maxNumOfExercises, difficulty)
maxSets = numMaxModifier(minNumOfSets, maxNumOfSets, difficulty)
maxExercisesOLD = numMaxModifierOLD(minNumOfExercises, maxNumOfExercises, difficulty)
maxSetsOLD = numMaxModifierOLD(minNumOfSets, maxNumOfSets, difficulty)
maxReps = numOfRepsMax(toughness, difficulty)

#Set all of the minimum values for plotting.
minExercises = numMinModifier(minNumOfExercises, maxNumOfExercises, difficulty)
minSets = numMinModifier(minNumOfSets, maxNumOfSets, difficulty)
minExercisesOLD = numMinModifierOLD(minNumOfExercises, maxNumOfExercises, difficulty)
minSetsOLD = numMinModifierOLD(minNumOfSets, maxNumOfSets, difficulty)
minReps = numOfRepsMin(toughness, difficulty)

#Convert values to real live relative difficulties.
actDifficultyAve = actualDifficulty(aveExercises, aveReps, aveSets)
actDifficultyAveOLD = actualDifficulty(aveExercisesOLD, aveReps, aveSetsOLD)
actDifficultyMax = actualDifficulty(maxExercises, maxReps, maxSets)
actDifficultyMaxOLD = actualDifficulty(maxExercisesOLD, maxReps, maxSetsOLD)
actDifficultyMin = actualDifficulty(minExercises, minReps, minSets)
actDifficultyMinOLD = actualDifficulty(minExercisesOLD, minReps, minSetsOLD)
	
#Plot the number of exercises per difficulty.
plt.xlabel('Difficulty')
plt.ylabel('Number of Exercises')
plt.title('Exercises (Per Set) vs Difficulty in MIA')
#plt.axhline(y=maxNumOfExercises,color='black', linestyle='-.',label='maxNumOfExercises')
plt.plot(difficulty, aveExercises,color="red", linestyle='--', label='Average Exercises')
plt.plot(difficulty, maxExercises,color="green", label='Maximum Exercises')
plt.plot(difficulty, maxExercisesOLD,color="green", linestyle = ':', label='Old Maximim Values')
plt.plot(difficulty, minExercises,color="blue", label='Minimum Exercises')
plt.plot(difficulty, minExercisesOLD,color="blue", linestyle = ':', label='Old Minimum Values')
plt.fill_between(difficulty, maxExercises, minExercises, color='grey', alpha='0.2')
plt.legend()
plt.show()

#Plots the number of sets per difficulty.
plt.xlabel('Difficulty')
plt.ylabel('Number of Sets')
plt.title('Sets vs Difficulty in MIA')
#plt.axhline(y=maxNumOfSets,color='black', linestyle='-.',label='maxNumOfSets')
plt.plot(difficulty, aveSets,color="red", linestyle='--', label='Average Sets')
plt.plot(difficulty, maxSets,color="green", label='Maximum Sets')
plt.plot(difficulty, maxSetsOLD,color="green", linestyle = ':', label='Old Maximim Values')
plt.plot(difficulty, minSets,color="blue", label='Minimum Sets')
plt.plot(difficulty, minSetsOLD,color="blue", linestyle = ':', label='Old Minimum Values')
plt.fill_between(difficulty, maxSets, minSets, color='grey', alpha='0.2')
plt.legend()
plt.show()

#Plots the number of reps per workout per difficulty.
plt.xlabel('Difficulty')
plt.ylabel('Number of Reps Per Workout')
plt.title('Reps vs Difficulty in MIA (weight='+str(weight)+')')
plt.plot(difficulty, aveReps,color="red", linestyle='--', label='Average Reps')
plt.plot(difficulty, maxReps,color="green", label='Maximum Reps')
plt.plot(difficulty, minReps,color="blue", label='Minimum Reps')
plt.fill_between(difficulty, maxReps, minReps, color='grey', alpha='0.2')
plt.legend()
plt.show()
	
#Plots the real world difficulties for new and old values.
plt.xlabel('Input Difficulty Parameter')
plt.ylabel('Relative Real World Workout Difficulty')
plt.title('Input vs Real World MIA Workout Difficulties')
plt.plot(difficulty, actDifficultyAve,color="black", linestyle='--', label='Average Difficulty')
#plt.plot(difficulty, actDifficultyAveOLD,color="red", linestyle=':')
plt.plot(difficulty, actDifficultyMax,color="green", label='Maximum Difficulty')
#plt.plot(difficulty, actDifficultyMaxOLD,color="green", linestyle=':', label='Old Maximim Values')
plt.plot(difficulty, actDifficultyMin,color="blue", label='Minimum Difficulty')
#plt.plot(difficulty, actDifficultyMinOLD,color="blue", linestyle=':', label='Old Minimum Values')
plt.fill_between(difficulty, actDifficultyMax, actDifficultyMin, where=difficulty>=0, color='0.9', alpha='1')
plt.fill_between(difficulty, actDifficultyMax, actDifficultyMin, where=difficulty>=11, color='0.75', alpha='1')
plt.fill_between(difficulty, actDifficultyMax, actDifficultyMin, where=difficulty>=25, color='0.6', alpha='1')
plt.fill_between(difficulty, actDifficultyMax, actDifficultyMin, where=difficulty>=39, color='0.45', alpha='1')
plt.fill_between(difficulty, actDifficultyMax, actDifficultyMin, where=difficulty>=54, color='0.3', alpha='1')
plt.fill_between(difficulty, actDifficultyMax, actDifficultyMin, where=difficulty>=75, color='0.15', alpha='1')


plt.legend()
plt.show()


	