#pragma once

#include <QDialog>
#include <QString>

// Modal box reporting a crash, with the message in a copyable text box
class CrashDialog : public QDialog
{
	Q_OBJECT
public:
	explicit CrashDialog(const QString& message, QWidget* parent = nullptr);
};
