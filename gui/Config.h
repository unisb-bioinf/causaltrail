#ifndef GUI_CONFIG_H
#define GUI_CONFIG_H

#include <QtCore/QString>
#include <QtCore/QSettings>

class Config
{
	public:
	Config();
	~Config();

	const QString dataDir() const;
	void setDataDir(const QString&);

	void save();

	private:
	QString defaultDataDir() const;
	QSettings settings_;
};

#endif // GUI_CONFIG_H
