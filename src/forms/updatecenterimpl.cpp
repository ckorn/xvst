#include "updatecenterimpl.h"
//
UpdateCenterImpl::UpdateCenterImpl(Updates *updates, bool autoDownloadAndInstall, QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
	// version
	lblxVSTVersion->setText(QString(lblxVSTVersion->text()).arg(PROGRAM_VERSION));
	// set update class
	this->updates = updates;
	// configure updates list
	QStringList headers;
	headers << tr(" File ") << tr(" Version ") << tr(" Size ") << tr(" Progress ");
	// add the headers
	lsvUpdates->setHeaderLabels(headers);
	// change headers sizes
	QFontMetrics fm = fontMetrics();
	QHeaderView *header = lsvUpdates->header();
	// resize
	header->resizeSection(1, fm.width(headers.at(0) + "9.99.999 alpha"));
	header->resizeSection(2, fm.width(headers.at(1) + " 1024,99Mb "));
	// configure resize mode
	header->setHighlightSections(false);
	header->setStretchLastSection(false);
	header->setResizeMode(0, QHeaderView::Stretch);
	// set header text aligment
	QTreeWidgetItem * headerItem = lsvUpdates->headerItem();
	headerItem->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(2, Qt::AlignRight   | Qt::AlignVCenter);
	headerItem->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
	// fill data
	fillUpdates();
	// signals
	connect(lsvUpdates, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*, int)));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));
	connect(btnUpdate, SIGNAL(clicked()), this, SLOT(btnUpdateClicked()));
	// updater
	connect(updates, SIGNAL(downloadingUpdate(int, int, int)), this, SLOT(downloadingUpdate(int, int, int)));
	connect(updates, SIGNAL(downloadFinished(int)), this, SLOT(downloadFinished(int)));
	connect(updates, SIGNAL(downloadsFinished()), this, SLOT(downloadsFinished()));
	connect(updates, SIGNAL(readyToInstallUpdates()), this, SLOT(readyToInstallUpdates()));
	// if auto download & install updates, then...
	if (autoDownloadAndInstall)
		btnUpdateClicked();
}

void UpdateCenterImpl::fillUpdates()
{
	for (int n = 0; n < updates->getUpdatesCount(); n++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(lsvUpdates);

		item->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
		item->setTextAlignment(2, Qt::AlignRight   | Qt::AlignVCenter);
		item->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);

		item->setText(0, updates->getUpdateItem(n)->getCaption());
		item->setText(1, updates->getUpdateItem(n)->getVersion());
		item->setText(2, fileSizeToString(updates->getUpdateItem(n)->getSize()));
		item->setText(3, "-");
		
		item->setSizeHint(0, QSize(18,18));
		
		item->setCheckState(0, Qt::Checked);
	}
}

void UpdateCenterImpl::itemChanged(QTreeWidgetItem * item, int column)
{
	if (lsvUpdates->isEnabled())
	{
		bool enabled = false;
	
		for (int n = 0; n < lsvUpdates->topLevelItemCount(); n++)
			if (lsvUpdates->topLevelItem(n)->checkState(0) == Qt::Checked)
				enabled = true;
		
		btnUpdate->setEnabled(enabled);
	}
}

void UpdateCenterImpl::btnCancelClicked()
{
	done(QDialog::Rejected);
}

void UpdateCenterImpl::btnUpdateClicked()
{
	lsvUpdates->setEnabled(false);
	btnUpdate->setEnabled(false);
	updates->downloadUpdates();
}

void UpdateCenterImpl::downloadingUpdate(int updateIndex, int pogress, int totalProgress)
{
	lsvUpdates->topLevelItem(updateIndex)->setText(3, QString("%1%").arg(pogress));
	pgbUpdate->setValue(totalProgress);
	lblUpdateSate->setText(tr("<b>Update state:</b> Downloading %1")
							.arg(lsvUpdates->topLevelItem(updateIndex)->text(0)));
	lblDownloadedSize->setText(tr("%1 (%2)")
								.arg(fileSizeToString(updates->getCurrentDownloaded()))
								.arg(fileSizeToString(updates->getTotalToDownload())));
}

void UpdateCenterImpl::downloadFinished(int updateIndex)
{
	lsvUpdates->topLevelItem(updateIndex)->setText(3, tr("Done"));
}

void UpdateCenterImpl::downloadsFinished()
{
	btnCancel->setEnabled(false);
}

void UpdateCenterImpl::readyToInstallUpdates()
{
	updates->installUpdates();
	// restart the xVST
	setResult(QDialog::Accepted);
	QApplication::closeAllWindows();
}
//
