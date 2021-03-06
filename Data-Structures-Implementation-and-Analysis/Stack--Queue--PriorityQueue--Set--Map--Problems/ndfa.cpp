// Submitter: cmorte(Morte, Christian)

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


typedef ics::ArraySet<std::string>                     States;
typedef ics::ArrayQueue<std::string>                   InputsQueue;
typedef ics::ArrayMap<std::string,States>              InputStatesMap;

typedef ics::ArrayMap<std::string,InputStatesMap>       NDFA;
typedef ics::pair<std::string,InputStatesMap>           NDFAEntry;

bool gt_NDFAEntry (const NDFAEntry& a, const NDFAEntry& b)
{return a.first<b.first;}

typedef ics::ArrayPriorityQueue<NDFAEntry,gt_NDFAEntry> NDFAPQ;

typedef ics::pair<std::string,States>                   Transitions;
typedef ics::ArrayQueue<Transitions>                    TransitionsQueue;


//Read an open file describing the non-deterministic finite automaton (each
//  line starts with a state name followed by pairs of transitions from that
//  state: (input followed by a new state, all separated by semicolons), and
//  return a Map whose keys are states and whose associated values are another
//  Map with each input in that state (keys) and the resulting set of states it
//  can lead to.
const NDFA read_ndfa(std::ifstream &file) {
	NDFA ndfa;
	std::string line;

	while (getline(file, line)){
		std::vector<std::string> words = ics::split(line, ";");
		const std::string& transition = words[0];
		std::string input;
		int index = 1;

		if (words.size() == 1) {
			ndfa[transition] = InputStatesMap();
			continue;
		}

		for (auto i = words.begin() + 1; i != words.end(); ++i) {
			if (index % 2 == 1) {
				input = *i;
			} else {
				ndfa[transition][input].insert(*i);
			}
			++index;
		}
	}
	return ndfa;
}


//Print a label and all the entries in the finite automaton Map, in
//  alphabetical order of the states: each line has a state, the text
//  "transitions:" and the Map of its transitions.
void print_ndfa(const NDFA& ndfa) {
	NDFAPQ ndfaPQ(gt_NDFAEntry);
	ndfaPQ.enqueue_all(ndfa);

	std::cout << "Non-Deterministic Finite Automaton Description" << std::endl;
	for (const NDFAEntry& entry : ndfaPQ) {
		std::cout << "  " << entry.first << " transitions: " << entry.second << std::endl;
	}
}


//Return a queue of the calculated transition pairs, based on the non-deterministic
//  finite automaton, initial state, and queue of inputs; each pair in the returned
//  queue is of the form: input, set of new states.
//The first pair contains "" as the input and the initial state.
//If any input i is illegal (does not lead to any state in the non-deterministic finite
//  automaton), ignore it.
TransitionsQueue process(const NDFA& ndfa, std::string state, const InputsQueue& inputs) {
	TransitionsQueue transitions;
	States states{state};

	transitions.enqueue( Transitions("", states) );
	for (const std::string& input : inputs) {
		States newStates;

		for (const std::string& state : states) {
			if (ndfa[state].has_key(input)) {
				newStates.insert_all(ndfa[state][input]);
			}
		}
		states = newStates;
		transitions.enqueue( Transitions(input, states) );
	}
	return transitions;
}


//Print a TransitionsQueue (the result of calling process) in a nice form.
//Print the Start state on the first line; then print each input and the
//  resulting new states indented on subsequent lines; on the last line, print
//  the Stop state.
void interpret(TransitionsQueue& tq) {  //or TransitionsQueue or TransitionsQueue&&
	std::cout << "Start state = " << tq.dequeue().second << std::endl;
	while (tq.size() > 0) {
		const Transitions& transition = tq.dequeue();
		std::cout << "  Input = " << transition.first << "; new states = " << transition.second << std::endl;

		if (tq.size() == 0) { std::cout << "Stop state(s) = " << transition.second << std::endl;}
	}
}



//Prompt the user for a file, create a finite automaton Map, and print it.
//Prompt the user for a file containing any number of simulation descriptions
//  for the finite automaton to process, one description per line; each
//  description contains a start state followed by its inputs, all separated by
//  semicolons.
//Repeatedly read a description, print that description, put each input in a
//  Queue, process the Queue and print the results in a nice form.
int main() {
  try {
	std::ifstream ndfa_file;
	ics::safe_open(ndfa_file,"Enter the name of a file with a Non-Deterministic Finite Automaton","ndfaendin01.txt");

	const NDFA ndfa = read_ndfa(ndfa_file);
	print_ndfa(ndfa);
	std::cout << std::endl;

	std::ifstream inputs_file;
	std::string line;
	ics::safe_open(inputs_file,"Enter the name of a file with the start-states and input","ndfainputendin01.txt");


	while (getline(inputs_file, line)){
		InputsQueue inputsQueue;
		std::vector<std::string> words = ics::split(line, ";");
		const std::string& startState = words[0];


		std::cout << std::endl << "Starting new simulation with description: " << line;

		for (auto i = words.begin() + 1; i != words.end(); i++) {
			inputsQueue.enqueue(*i);
		}

		TransitionsQueue transitions = process(ndfa, startState, inputsQueue);
		interpret(transitions);
	}

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
