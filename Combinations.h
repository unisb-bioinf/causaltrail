#ifndef COMBINATIONS_H
#define COMBINATIONS_H
#include <vector>
#include <unordered_map>
#include <iostream>

template<typename T>
class Combinations{
	public:
	Combinations(const std::vector<unsigned int>& keys, const std::vector<std::vector<T>>& values);
	void createCombinations(unsigned int position);
	std::vector<std::vector<T>> getResult();
	private:
	const std::vector<unsigned int>& keys_;
	const std::vector<std::vector<T>>& values_;
	std::vector<T> temp_;
	std::vector<std::vector<T>> result_;
};
#endif


/**Combinations()
 *
 * @param keys a vector containing a key for each exchangeable position
 * @param values a map that holds the possible values for each key
 *
 * @return a Combinations Object
 *
 * This class creates all enumerations for the enumeration problem without replacement and without regard to sequence
 */
template<typename T>
Combinations<T>::Combinations(const std::vector<unsigned int>& keys, const std::vector<std::vector<T>>& values)
	:keys_(keys),values_(values)
	{
	temp_.resize(values.size());
	}

/**createCombinations
 *
 * @param
 * @param
 *
 * @return void
 *
 * Recursively creates a enumeration of all possible combinations given the keys and their possible values
 */
template<typename T>
void Combinations<T>::createCombinations(unsigned int position=0){
	if (position == keys_.size()){
		result_.push_back(temp_);
		}
	else {
		for (auto& v : values_[keys_[position]]){
			temp_[keys_[position]]=v;
			createCombinations(position+1);
			}
		}	
	}


/**getResult
 *
 * @return A vector containing all possible combinations
 *
 * Returns the enumeration result
 */
template<typename T>
std::vector<std::vector<T>> Combinations<T>::getResult(){
	return result_;
	}

