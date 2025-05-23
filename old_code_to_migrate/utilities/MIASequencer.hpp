/**
 * File: MIASequencer.hpp
 * Author: Antonius Torode
 * Date: 12/26/2019
 * Description: MIA sequencer for processing the MIASequences file.
 */

#include <string>
#include <vector>
#include <map>

#include "VirtualKeyStrokes.hpp"

using std::string;
using std::vector;
using std::map;

class [[maybe_unused]] Sequencer
{
	private:
		//Variables for loading the sequence file.
		int lastSequenceIndex = 0;
		int sequenceSize = 0;
		vector<string> sequenceNames;
		vector<int> sequenceXCoordLine;
		vector<int> sequenceYCoordLine;
		vector<string> sequenceActionLine;
		
		//Maps for storing sequencer data.
		map<string, int> sequenceTimings;
		map<string, int> sequenceHoverTimes;
		map<string, int> sequenceSizes;
		map<string, vector<int>> sequenceXCoords;
		map<string, vector<int>> sequenceYCoords;
		map<string, vector<string>> sequenceActions;

		VirtualKeyStrokes keys;

	public:

		//Functions for initializing the sequencer and loading in variables from file.

		/**
		 * Main default constructor. Loads in default variables and initializes the sequences.
		 */
		Sequencer();

		/**
		 * Main default destructor.
		 */
		~Sequencer() = default;

		/**
		 * This will set an appropriate sequence value based on the input.
		 * @param variable is the sequencer variable type to use.
		 * @param value is the value of the sequencer variable.
		 */
		void setSequenceVariables(string variable, string value);

		/**
		 * Set's all the variables from the MIA Sequences file.
		 */
		void initializeSequences();
		
		//Functions for handling sequencer data.

		/**
		 * Extracts the size of a sequence from a sequence name.
		 * @param sequence - the sequence name to use.
		 * @return int size of the sequence.
		 */
		int getSequenceSize(const string& sequence);

		/**
		 * Extracts the x coordinates from a sequence name.
		 * @param sequence - the sequence name to use.
		 * @return all x coords as a vector<int>.
		 */
		vector<int> getSequenceXCoords(const string& sequence);

        /**
         * Extracts the y coordinates from a sequence name.
         * @param sequence - the sequence name to use.
         * @return all y coords as a vector<int>.
         */
		vector<int> getSequenceYCoords(const string& sequence);

		/**
		 * Extracts the actions from a sequence name.
		 * @param sequence - the sequence name to use.
		 * @return all actions as a vector<std::string>.
		 */
		vector<string> getSequenceActions(const string& sequence);
		vector<string> getSequenceNames()
		{ return sequenceNames; };
		
		//Functions for using the sequencer.
		/**
		 * Activates and performs a sequence.
		 * @param sequence - the sequence name to use.
		 */
		void activateSequence(const string& sequence);
};
