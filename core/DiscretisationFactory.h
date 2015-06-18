#ifndef DISCRETISATIONFACTORY_H
#define DISCRETISATIONFACTORY_H

#include "DiscretisationSettings.h"
#include "Discretisations.h"

#include <memory>
#include <unordered_map>

class DiscretisationFactory
{
	public:
	DiscretisationFactory(const DiscretisationSettings& jsonTree);

	std::unique_ptr<Discretisations> create(const std::string& nodeName);

	private:
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
