#ifndef OPTIONSIMPL_H
#define OPTIONSIMPL_H
//
#include <QtGui>
//
#include "ui_options.h"
//
#include "downloadlogimpl.h"
//
#include "../options.h"
#include "../sessionmanager.h"
#include "../videoinformation.h"
#include "../languages.h"
//
class OptionsImpl : public QDialog, public Ui::Options
{
Q_OBJECT
	private:
		ProgramOptions *programOptions;
		SessionManager *sessionManager;
		VideoInformation *videoInformation;
		LanguageManager *languageManager;
		int lastPageViewed;
		QString tmpLangFile;
		void createMenu();
		void fillInitialData();
		void fillLanguages();
		void setInitialOptionsValues();
		void setOptionsValues();
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
	public:
		OptionsImpl(ProgramOptions *programOptions, SessionManager *sessionManager,
			VideoInformation *videoInformation, int lastOptionsPage, 
			QWidget * parent = 0, Qt::WFlags f = 0);
		~OptionsImpl();
		int getLastPageViewed();
	private slots:
		void menuCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
		void langCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
		void btnOkClicked();
		void ffmpegTextChanged(const QString &text);
		void btnViewLogClicked();
		void btnClearLogClicked();
		void chbSaveRestoreStateClicked(bool checked = false);
		void spbRemovePressed();
		void spbRemoveAllPressed();
		void spbAddAllPressed();
		void spbAddPressed();
		void servicesItemSelectionChanged();
		void btnUseThisClicked();
		void langItemDoubleClicked(QTreeWidgetItem *item, int column);
};
#endif
