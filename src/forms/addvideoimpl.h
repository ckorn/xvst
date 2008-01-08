#ifndef ADDVIDEOIMPL_H
#define ADDVIDEOIMPL_H
//
#include "ui_addvideo.h"
//
#include "../videoinformation.h"
//
class AddVideoImpl : public QDialog, public Ui::AddVideo
{
Q_OBJECT
	private:
		VideoInformation *videoInformation;
	public:
		AddVideoImpl(VideoInformation *videoInformation, QWidget * parent = 0, Qt::WFlags f = 0 );
	private slots:
		void btnOkClicked();
		void edtURLChanged(const QString &text);
};
#endif
