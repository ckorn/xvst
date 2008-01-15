#ifndef UPDATECENTERIMPL_H
#define UPDATECENTERIMPL_H
//
#include <QDialog>
//
#include "ui_updatecenter.h"
//
#include "../updates.h"
#include "../tools.h"
#include "../programversion.h"

class UpdateCenterImpl : public QDialog, public Ui::UpdateCenter
{
Q_OBJECT
	private:
		Updates *updates;
		void fillUpdates();
	public:
		UpdateCenterImpl(Updates *updates, bool autoDownloadAndInstall, QWidget * parent = 0, Qt::WFlags f = 0 );
	private slots:
		void itemChanged(QTreeWidgetItem * item, int column);
		void btnCancelClicked();
		void btnUpdateClicked();
		// updater
		void downloadingUpdate(int updateIndex, int pogress, int totalProgress);
		void downloadFinished(int updateIndex);
		void downloadsFinished();
		void readyToInstallUpdates();
};
#endif
