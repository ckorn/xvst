#ifndef CUSTOMALPHABLENDIMPL_H
#define CUSTOMALPHABLENDIMPL_H
//
#include "ui_customalphablend.h"
//
class CustomAlphaBlendImpl : public QDialog, public Ui::CustomAlphaBlend
{
Q_OBJECT
	public:
		CustomAlphaBlendImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		void setInitialAlphaBlend(float value);
		float getAlphaBlend();
	private slots:
		void okClicked();
		void cancelClicked();
		void hsAlphaBlendvalueChanged(int value);
};
#endif
