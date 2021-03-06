#ifndef DISCRETISER_H
#define DISCRETISER_H

#include "Discretisations.h"
#include "DiscretisationSettings.h"
#include "Network.h"
#include "float.h"
#include <map>

class Discretiser
{
	public:
	/**Discretiser
	 *
	 * @param originalObservations, a const reference to the matrix containing
	 * the raw sample data
	 * @param obsMatrix, a reference to the new observation matrix that shall
	 * contain the discretised data
	 * @param network, a reference to the network
	 *
	 * @return Discretiser Object
	 */
	Discretiser(const Matrix<std::string>& originalObservations,
	            Matrix<int>& obsMatrix, Network& network);

	/**Discretiser
	 *
	 * @param originalObservations, a const reference to the matrix containing
	 * the raw sample data
	 * @param filename, name of a "controlFile" that regulates the
	 * discretisation for each node
	 * @param obsMatrix, a reference to the new observation matrix that shall
	 * contain the discretised data
	 * @param network, a reference to the network
	 *
	 * @return Discretiser Object
	 *
	 * In contrast to the before declared constructor, this constructor uses the
	 * controlFile and automatically discretises
	 * all observations that are listed in this file
	 */
	Discretiser(const Matrix<std::string>& originalObservations,
	            const std::string& filename, Matrix<int>& obsMatrix,
	            Network& network);

	/**setJsonTree
	 *
	 * @param A reference to a DiscretisationSettings Object
	 * 
	 * Sets DiscretisationSettings constructed in DiscretisationSelection
	 */
	void setJsonTree(const DiscretisationSettings&);

	Discretiser& operator=(const Discretiser&) = delete;

	Discretiser& operator=(Discretiser&&) = delete;

	/**discretise
	 *
	 * @param const std::string& controlFile name of the json file 
	 * containing the discretisation information
	 *
	 * Extracts the discretisation information from the controlFile
	 * and calls discretise() subsequently 
	 */
	void discretise(const std::string& controlFile);

	/**discretise
	 * 
	 * Calls the apply function of all Discretisations objects stored in 
	 * discretisations_
	 */
	void discretise();

	private:
	/**createDiscretisaionClasses
	 *
	 * @param controlFile
	 * 
	 * Generates Discretisations objects according to the specified controlFile
	 */
	void createDiscretisationClasses(const std::string& controlFile);

	/**adaptFormat
	 *
	 * Replaces various kinds of NA representations by a unqiue version
	 */
	void adaptFormat();
	// Json Tree
	DiscretisationSettings jsonTree_;
	// Matrix containing the original raw sample data
	Matrix<std::string> originalObservations_;
	// Matrix containing the discretised data
	Matrix<int>& observations_;
	// Vector of unique pointers, pointing to discretisation objects
	std::vector<std::unique_ptr<Discretisations>> discretisations_;
	// Network
	Network& network_;
};
#endif
