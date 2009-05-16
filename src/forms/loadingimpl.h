#ifndef LOADINGIMPL_H
#define LOADINGIMPL_H

#include <QtGui>

#include "ui_loadingimpl.h"

class LoadingImpl : public QWidget, private Ui::LoadingImpl {
    Q_OBJECT
    Q_DISABLE_COPY(LoadingImpl)
public:
    explicit LoadingImpl(QWidget *parent = 0);
	void finished();
private slots:
	void closeLoading();
};

#endif // LOADINGIMPL_H
