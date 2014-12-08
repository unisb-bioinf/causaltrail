#ifndef FACTOR_H
#define FACTOR_H

#include "Network.h"

class Factor{
	public:
	Factor(const Node& n, const std::vector<int>& values);
	Factor(unsigned int length, std::vector<unsigned int> ids);
	const std::vector<unsigned int>& getIDs() const ;
	const std::vector<std::vector<int>>& getAllValues() const;
	const std::vector<int>& getValues(unsigned int index) const;
	const int& getValue(unsigned int row, unsigned int index) const;
	void addValue(const std::vector<int>& value);
	void setValue(const std::vector<int>& value, unsigned int index);
	void addProbability(float prob);
	void setProbability(float prob, unsigned int index);
	float getProbability(unsigned int index) const;
	float getProbability(const std::vector<int>& values) const;
	unsigned int getIndex(unsigned int index) const;
	Factor product(Factor& factor,const Network& network_, const std::vector<int>& values);
	Factor sumOut(unsigned int id, Network& network_, const std::vector<int>& values);
	void normalize();
	friend std::ostream& operator<< (std::ostream& os,const Factor& f);
	
	private:
	std::vector<unsigned int> getUnionOfIDs(const std::vector<unsigned int>& commonIDs, const Factor& factor) const;
	std::vector<unsigned int> getCommonIDs(const Factor& factor) const;
	std::vector<unsigned int> nodeIDs_;
	std::vector<int> val_;
	std::vector<float> probabilities_;
	int length_;
};

#endif
