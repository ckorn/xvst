#include "customalphablendimpl.h"
//
CustomAlphaBlendImpl::CustomAlphaBlendImpl( QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
	// connect buttons
	connect(btnOk, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	// connect the SldeBar
	connect(hsAlphaBlend, SIGNAL(valueChanged(int)), this, SLOT(hsAlphaBlendvalueChanged(int)));
}

void CustomAlphaBlendImpl::setInitialAlphaBlend(float value)
{
	hsAlphaBlend->setValue(static_cast<int>(value * 100));
	hsAlphaBlendvalueChanged(hsAlphaBlend->value());
}

float CustomAlphaBlendImpl::getAlphaBlend()
{
	if (hsAlphaBlend->value() <= 0)
		return 0.01;
	else
		return static_cast<float>(hsAlphaBlend->value()) / 100;
}

void CustomAlphaBlendImpl::okClicked()
{
	accept();
}

void CustomAlphaBlendImpl::cancelClicked()
{
	reject();
}

void CustomAlphaBlendImpl::hsAlphaBlendvalueChanged(int value)
{
	if (value == 0) value = 1;
	lblCurrentValue->setText(tr("Current: <b>%1%</b>").arg(value));
}
//

