#include "Config.h"

#include <QtCore/QStandardPaths>

Config::Config() : settings_("bioinf.uni-sb.de", "CausalTrail") {}

Config::~Config() { save(); }

const QString Config::dataDir() const
{
	auto value = settings_.value("dataDirectory", defaultDataDir());

	return value.toString();
}

void Config::setDataDir(const QString& path)
{
	settings_.setValue("dataDirectory", path);
}

void Config::save()
{
	if(settings_.isWritable()) {
		settings_.sync();
	}
}

QString Config::defaultDataDir() const
{
	return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}
