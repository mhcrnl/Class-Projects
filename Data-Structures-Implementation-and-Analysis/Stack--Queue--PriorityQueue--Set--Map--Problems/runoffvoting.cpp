//// Submitter: cmorte(Morte, Christian)
//
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
//#include "ics46goody.hpp"
//#include "array_queue.hpp"
//#include "array_priority_queue.hpp"
//#include "array_set.hpp"
//#include "array_map.hpp"
//
//
//typedef ics::ArrayQueue<std::string>              CandidateQueue;
//typedef ics::ArraySet<std::string>                CandidateSet;
//typedef ics::ArrayMap<std::string,int>            CandidateTally;
//
//typedef ics::ArrayMap<std::string,CandidateQueue> Preferences;
//typedef ics::pair<std::string,CandidateQueue>     PreferencesEntry;
//typedef ics::ArrayPriorityQueue<PreferencesEntry> PreferencesEntryPQ; //Must supply gt at construction
//
//typedef ics::pair<std::string,int>                TallyEntry;
//typedef ics::ArrayPriorityQueue<TallyEntry>       TallyEntryPQ;
//
//bool preferences_gt (const PreferencesEntry& a, const PreferencesEntry& b)
//{return a.first<b.first;}
//
//bool alphabetic_gt(const TallyEntry& a, const TallyEntry& b) {
//	return a.first < b.first;
//}
//bool numeric_gt(const TallyEntry& a, const TallyEntry& b) {
//	return a.second == b.second ? alphabetic_gt(a, b) : a.second > b.second;
//}
//
////Read an open file stating voter preferences (each line is (a) a voter
////  followed by (b) all the candidates the voter would vote for, in
////  preference order (from most to least preferred candidate, separated
////  by semicolons), and return a Map of preferences: a Map whose keys are
////  voter names and whose values are a queue of candidate preferences.
//Preferences read_voter_preferences(std::ifstream &file) {
//	Preferences prefs;
//	std::string line;
//
//	while (getline(file, line)){
//		std::vector<std::string> words = ics::split(line, ";");
//		const std::string& voter = words[0];
//
//		for (auto i = words.begin() + 1; i != words.end(); i++){
//			prefs[voter].enqueue(*i);
//		}
//	}
//	return prefs;
//}
//
//
////Print a label and all the entries in the preferences Map, in alphabetical
////  order according to the voter.
////Use a "->" to separate the voter name from the Queue of candidates.
//void print_voter_preferences(const Preferences& preferences) {
//	PreferencesEntryPQ sortedPrefs(preferences_gt);
//	sortedPrefs.enqueue_all(preferences);
//
//	std::cout << "Voter Preferences" << std::endl;
//	for (const PreferencesEntry& prefEntry : sortedPrefs) {
//		std::cout << "  " << prefEntry.first << " -> " << prefEntry.second << std::endl;
//	}
//}
//
//
////Print the message followed by all the entries in the CandidateTally, in
////  the order specified by has_higher_priority: i is printed before j, if
////  has_higher_priority(i,j) returns true: sometimes alphabetically by candidate,
////  other times by decreasing votes for the candidate.
////Use a "->" to separate the candidate name from the number of votes they
////  received.
//void print_tally(std::string message, const CandidateTally& tally, bool (*has_higher_priority)(const TallyEntry& i,const TallyEntry& j)) {
//	CandidateSet candidateSet;
//	for (const auto& kv : tally) {
//		candidateSet.insert(kv.first);
//	}
//	TallyEntryPQ tallyPQ(*has_higher_priority);
//	tallyPQ.enqueue_all(tally);
//	std::cout << message << ": still in election = " << candidateSet << std::endl;
//
//	for (const TallyEntry& tallyEntry : tallyPQ) {
//		std::cout << "  " << tallyEntry.first << " -> " << tallyEntry.second << std::endl;
//	}
//	std::cout << std::endl;
//}
//
//
////Return the CandidateTally: a Map of candidates (as keys) and the number of
////  votes they received, based on the unchanging Preferences (read from the
////  file) and the candidates who are currently still in the election (which changes).
////Every possible candidate should appear as a key in the resulting tally.
////Each voter should tally one vote: for their highest-ranked candidate who is
////  still in the the election.
//CandidateTally evaluate_ballot(const Preferences& preferences, const CandidateSet& candidates) {
//	CandidateTally tally;
//	for (const PreferencesEntry& entry : preferences) {
//		for (const std::string& candidate : entry.second) {
//			if (candidates.contains(candidate)) {
//				++tally[candidate];
//				break;
//			}
//		}
//	}
//	return tally;
//}
//
//
////Return the Set of candidates who are still in the election, based on the
////  tally of votes: compute the minimum number of votes and return a Set of
////  all candidates receiving more than that minimum; if all candidates
////  receive the same number of votes (that would be the minimum), the empty
////  Set is returned.
//CandidateSet remaining_candidates(const CandidateTally& tally) {
//	CandidateSet result;
//	if (!tally.empty()) {
//		int min = -1;
//
//		for (const TallyEntry& entry : tally) {
//				if (min == -1 || entry.second < min) {
//					min = entry.second;
//				}
//		}
//		for (const TallyEntry& entry : tally) {
//			if (entry.second > min) {
//				result.insert(entry.first);
//			}
//		}
//	}
//	return result;
//}
//
//
////Prompt the user for a file, create a voter preference Map, and print it.
////Determine the Set of all the candidates in the election, from this Map.
////Repeatedly evaluate the ballot based on the candidates (still) in the
////  election, printing the vote count (tally) two ways: with the candidates
////  (a) shown alphabetically increasing and (b) shown with the vote count
////  decreasing (candidates with equal vote counts are shown alphabetically
////  increasing); from this tally, compute which candidates remain in the
////  election: all candidates receiving more than the minimum number of votes;
////  continue this process until there are less than 2 candidates.
////Print the final result: there may 1 candidate left (the winner) or 0 left
////   (no winner).
//int main() {
//  try {
//	  std::ifstream text_file;
//	  ics::safe_open(text_file,"Enter the name of a file with voter preferences","votepref1.txt");
//	  std::cout << std::endl;
//
//	  const Preferences prefs = read_voter_preferences(text_file);
//	  print_voter_preferences(prefs);
//	  std::cout << std::endl;
//
//	  CandidateSet candidateSet;
//	  for (const auto& kv : prefs) {
//		  for (const std::string& candidate : kv.second) {
//			  candidateSet.insert(candidate);
//		  }
//	  }
//
//	  CandidateTally tally = evaluate_ballot(prefs, candidateSet);
//	  int ballot = 1;
//
//	  while (candidateSet.size() > 1) {
//		  std::stringstream ss;
//		  ss << ballot++;
//		  print_tally("Vote count on ballot #" + ss.str() +
//				  " with candidates in alphabetical order", tally, alphabetic_gt);
//		  print_tally("Vote count on ballot #" + ss.str() +
//				  " with candidates in numerical order", tally, numeric_gt);
//
//		  candidateSet = remaining_candidates(tally);
//		  tally = evaluate_ballot(prefs, candidateSet);
//	  }
//
//	  if (candidateSet.size() == 0) {
//		  std::cout << "No winner" << std::endl;
//	  } else {
//		  std::string winner;
//		  for (const std::string& candidate : candidateSet) {
//			  winner += candidate + " and ";
//		  }
//		  std::cout << "Winner is " << winner.substr(0, winner.length()-5) << std::endl;
//	  }
//
//  } catch (ics::IcsError& e) {
//    std::cout << e.what() << std::endl;
//  }
//  return 0;
//}
