#ifndef COMBINATIONS_H
#define COMBINATIONS_H
#include <vector>
#include <unordered_map>
#include <iostream>

template<typename T>
class Combinations{
	public:
	/**Combinations()
	 *
	 * @param keys a vector containing a key for each exchangeable position
	 * @param values a map that holds the possible values for each key
	 *
	 * @return a Combinations Object
	 *
	 * This class creates all enumerations for the enumeration problem without replacement and without regard to sequence
	 */
	Combinations(const std::vector<unsigned int>& keys, const std::vector<std::vector<T>>& values);

	Combinations& operator=(const Combinations&) = delete;
	Combinations& operator=(Combinations&&) = delete;

	/**createCombinations
	 *
	 * @param position The position of the start key (0 if all enumeration should be constructed)
	 *
	 * Recursively creates a enumeration of all possible combinations given the keys and their possible values
	 */
	void createCombinations(unsigned int position = 0);

	/**getResult
	 *
	 * @return vector Containing all possible combinations
	 *
	 * Returns the enumeration result
	 */
	std::vector<std::vector<T>> getResult();

	private:

	//Vector to store the used keys
	const std::vector<unsigned int>& keys_;

	//Vector to store the possible values per key
	const std::vector<std::vector<T>>& values_;

	//Vector to store the temporal assignment
	std::vector<T> temp_;

	//Vector to store the entire enumeration
	std::vector<std::vector<T>> result_;
};
#endif

template<typename T>
Combinations<T>::Combinations(const std::vector<unsigned int>& keys, const std::vector<std::vector<T>>& values)
	:keys_(keys),values_(values)
	{
	temp_.resize(values.size());
	}

template<typename T>
void Combinations<T>::createCombinations(unsigned int position){
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

template<typename T>
std::vector<std::vector<T>> Combinations<T>::getResult(){
	return result_;
	}

