#ifndef DISCRETISATIONFACTORY_H
#define DISCRETISATIONFACTORY_H

#include <memory>
#include <unordered_map>

class Discretisations;
class DiscretisationSettings;
class DiscretiserParameters;

/**
 * A factory class that creates Discretisations as requested by the user.
 */
class DiscretisationFactory
{
	public:
	/**
	 * Constructs an instance of DiscretisationFactory for a given set
	 * settings.
	 *
	 * @param jsonTree The settings that should be used.
	 */
	explicit DiscretisationFactory(const DiscretisationSettings& jsonTree);

	/**
	 * Create a Discretisations instance
	 *
	 * @param nodeName The node for which the instance should be created.
	 * @return A valid pointer to a Discretisations instance
	 * @throws std::invalid_argument If the requested node or the method
	 * configured for the node are unknown.
	 * @throws boost::property_tree::ptree_bad_data if the provided settings
	 * contained incorrectly formatted data.
	 * @throws boost::property_tree::ptree_bad_path if a needed parameter
	 * could not be found.
	 */
	std::unique_ptr<Discretisations> create(const std::string& nodeName);

	private:
	// The following classes implement the type erasure pattern
	// for storing factory objects for the respective discretisation
	// methods.
	class Generator
	{
		public:
		virtual std::unique_ptr<Discretisations>
		operator()(const DiscretiserParameters&) const = 0;
		virtual ~Generator() = default;
	};

	template <typename T> class GeneratorModel : public Generator
	{
		public:
		GeneratorModel(const T& generator) : generator_(generator) {}

		std::unique_ptr<Discretisations>
		operator()(const DiscretiserParameters& params) const override
		{
			return generator_(params);
		}

		private:
		T generator_;
	};

	template <typename T> void insert(const std::string& id, const T& generator)
	{
		generators_.emplace(id, std::make_unique<GeneratorModel<T>>(generator));
	}

	std::unordered_map<std::string, std::unique_ptr<Generator>> generators_;

	const DiscretisationSettings& jsonTree_;
};

#endif
