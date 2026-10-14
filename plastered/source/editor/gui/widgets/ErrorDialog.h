#pragma once

#include <QDialog>
#include <QString>

// Modal box reporting a recoverable error; the editor keeps running once it is dismissed
class ErrorDialog : public QDialog
{
	Q_OBJECT
public:
	explicit ErrorDialog(const QString& message, const QString& title = "Error", QWidget* parent = nullptr);
};
