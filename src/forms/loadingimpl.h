#ifndef LOADINGIMPL_H
#define LOADINGIMPL_H

#include <QtGui>

#include "ui_loadingimpl.h"

class LoadingImpl : public QWidget, private Ui::LoadingImpl
{
    Q_OBJECT
    Q_DISABLE_COPY(LoadingImpl)
private:
	explicit LoadingImpl(QWidget *parent = 0);
public:
	void finished();
	static LoadingImpl* instance();
public slots:
	void closeLoading();
};

#endif // LOADINGIMPL_H
