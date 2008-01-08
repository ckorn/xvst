#include <QtGui>//<QApplication>
//
#include "forms/mainformimpl.h"
//
#include "options.h"
#include "languages.h"
//
int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	// get language file
	ProgramOptions *programOptions = new ProgramOptions(qApp->applicationDirPath());
	programOptions->load();
	QString qm = LanguageManager::get_qm_languageFile(programOptions->getLanguageFile(true));
	delete programOptions;
	
	// install translator
	QTranslator translator;
	translator.load(qm);
	app.installTranslator(&translator);

	MainFormImpl win;
	win.show();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
//
