#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "../programsettings.h"
#include "../../../Common/xvst-classes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	// set http global user agent
	Http::setGlobalUserAgent("xVST-DebugPlugins");
	// init settings
	ProgramSettings settings;
	settings.load();
	ui->edtPluginPathVideoInformation->setText(settings.getVideoInformationPlugin());
	ui->edtTestingURLVideoInformation->setText(settings.getVideoInformationURL());
	ui->edtPluginPathSearchVideos->setText(settings.getVideoSearchPlugin());
	ui->edtTestingKeyWords->setText(settings.getVideoSearchKeyWords());
	ui->edtImagePath->setText(settings.getPluginIcon());
}

MainWindow::~MainWindow()
{
	ProgramSettings settings;
	settings.setVideoInformationPlugin(ui->edtPluginPathVideoInformation->text());
	settings.setVideoInformationURL(ui->edtTestingURLVideoInformation->text());
	settings.setVideoSearchPlugin(ui->edtPluginPathSearchVideos->text());
	settings.setVideoSearchKeyWords(ui->edtTestingKeyWords->text());
	settings.setPluginIcon(ui->edtImagePath->text());
	settings.save();
	//
	delete ui;
}

void MainWindow::testPluginVideoInformation(bool debug)
{
	ui->outputVideoInformation->clear();
	ui->outputVideoInformation->appendHtml("Getting video information... please wait...");

	// configure plugin
	VideoInformationPlugin plugin(NULL, ui->edtPluginPathVideoInformation->text());
	plugin.setDebug(debug);

	// execute plugin
	VideoDefinition vd = plugin.getVideoInformation(ui->edtTestingURLVideoInformation->text());

	// display information

	ui->imgIcon->setPixmap(*plugin.getIcon());

	ui->outputVideoInformation->clear();

	// plugin information

	ui->outputVideoInformation->appendHtml("<u><font size='+1'>Plugin information:</font></u>");
	ui->outputVideoInformation->appendHtml("<p>");
	ui->outputVideoInformation->appendHtml(QString("<b>Name:</b> %1").arg(plugin.getCaption()));
	ui->outputVideoInformation->appendHtml(QString("<b>Version:</b> %1").arg(plugin.getVersion()));
	ui->outputVideoInformation->appendHtml(QString("<b>Adult content:</b> %1").arg(plugin.hasAdultContent() ? "Yes" : "No"));
	ui->outputVideoInformation->appendHtml(QString("<b>Loaded:</b> %1").arg(plugin.isLoaded() ? "Yes" : "No"));
	ui->outputVideoInformation->appendHtml("</p>");

	// video information

	ui->outputVideoInformation->appendHtml("<u><font size='+1'>Video information:</font></u>");
	ui->outputVideoInformation->appendHtml("<p>");

	// video information: main information

	if (vd.title.isEmpty())
		ui->outputVideoInformation->appendHtml("<b>Title:</b> <font color='red'>Missing...</font>");
	else // ok
		ui->outputVideoInformation->appendHtml(QString("<b>Title:</b> %1").arg(vd.title));

	if (vd.URL.isEmpty())
		ui->outputVideoInformation->appendHtml("<b>URL:</b> <font color='red'>Missing...</font>");
	else // ok
		ui->outputVideoInformation->appendHtml(QString("<b>URL:</b> %1").arg(vd.URL));

	// video information: optional information
	ui->outputVideoInformation->appendHtml("<p>");
	ui->outputVideoInformation->appendHtml(QString("<b>Extension:</b> %1").arg(vd.extension));
	ui->outputVideoInformation->appendHtml(QString("<b>Is Audio File:</b> %1").arg(vd.isAudioFile ? "Yes" : "No"));
	ui->outputVideoInformation->appendHtml(QString("<b>Need Login:</b> %1").arg(vd.needLogin ? "Yes" : "No"));
	ui->outputVideoInformation->appendHtml("<p>");
	ui->outputVideoInformation->appendHtml(QString("<b>Overrided HTTP User-Agent:</b> %1").arg(vd.userAgent));
	ui->outputVideoInformation->appendHtml(QString("<b>Extra HTTP headers:</b> %1").arg(vd.headers));
	ui->outputVideoInformation->appendHtml(QString("<b>Extra HTTP cookies:</b> %1").arg(vd.cookies));

	ui->outputVideoInformation->appendHtml("</p>");
}

void MainWindow::testPluginSearchVideos(bool debug)
{
	ui->outputSearchVideos->clear();
	ui->outputSearchVideos->appendHtml(QString("Searching videos (page %1)... please wait...").arg(ui->spinBoxTestingPage->value()));

	// configure plugin
	VideoInformationPlugin plugin(NULL, ui->edtPluginPathSearchVideos->text());
	plugin.setDebug(debug);

	// execute plugin
	SearchResults sr = plugin.searchVideos(ui->edtTestingKeyWords->text(), ui->spinBoxTestingPage->value());

	ui->outputSearchVideos->clear();

	// plugin information

	ui->outputSearchVideos->appendHtml("<u><font size='+1'>Plugin information:</font></u>");
	ui->outputSearchVideos->appendHtml("<p>");
	ui->outputSearchVideos->appendHtml(QString("<b>Name:</b> %1").arg(plugin.getCaption()));
	ui->outputSearchVideos->appendHtml(QString("<b>Version:</b> %1").arg(plugin.getVersion()));
	ui->outputSearchVideos->appendHtml(QString("<b>Loaded:</b> %1").arg(plugin.isLoaded() ? "Yes" : "No"));
	ui->outputSearchVideos->appendHtml("</p>");

	// searchs information

	ui->outputSearchVideos->appendHtml("<u><font size='+1'>Search information:</font></u>");
	ui->outputSearchVideos->appendHtml("<p>");
	ui->outputSearchVideos->appendHtml(QString("<b>Summary:</b> %1").arg(sr.getSummary()));
	ui->outputSearchVideos->appendHtml(QString("<b>Results count:</b> %1").arg(sr.getSearchResultCount()));
	ui->outputSearchVideos->appendHtml("</p>");

	// searchs results

	ui->outputSearchVideos->appendHtml("<u><font size='+1'>Search results:</font></u>");
	ui->outputSearchVideos->appendHtml("<p>");
	for (int n = 0; n < sr.getSearchResultCount(); n++)
	{
		ui->outputSearchVideos->appendHtml(QString("<b>Title:</b> %1").arg(sr.getSearchResult(n)->getTitle()));
		ui->outputSearchVideos->appendHtml(QString("<b>Description:</b> %1").arg(sr.getSearchResult(n)->getDescription()));
		ui->outputSearchVideos->appendHtml(QString("<b>Video URL:</b> %1").arg(sr.getSearchResult(n)->getVideoUrl()));
		ui->outputSearchVideos->appendHtml(QString("<b>Preview URL:</b> %1").arg(sr.getSearchResult(n)->getImageUrl()));
		ui->outputSearchVideos->appendHtml(QString("<b>Rating:</b> %1").arg(sr.getSearchResult(n)->getRating()));
		ui->outputSearchVideos->appendHtml(QString("<b>Duration:</b> %1").arg(sr.getSearchResult(n)->getDuration()));
		ui->outputSearchVideos->appendHtml("<br>");
	}
	ui->outputSearchVideos->appendHtml("</p>");

}

void MainWindow::generateBinaryArray()
{
	ui->outputIconHex->clear();

	QFile file(ui->edtImagePath->text());
	if (file.exists())
	{
		// convert each byte to hex representation
		file.open(QIODevice::ReadOnly);
		QString hexData = file.readAll().toHex();
		file.close();
		// convert to JS array
		QString arrayData("\t\t");
		int len = 0;
		for (int n = 0; n < hexData.size(); n+=2)
		{
			arrayData = arrayData + "0x" + hexData.at(n) + hexData.at(n + 1) + ",";
			len++;
			if (len > 15) {	len = 0; arrayData += "\n\t\t"; }
		}
		// remove last ","
		arrayData.resize(arrayData.size() - 1);
		// generate function and array
		arrayData = QString("function getVideoServiceIcon()\n{\n\treturn new Array(\n%1);\n}").arg(arrayData);
		// print generated function
		ui->outputIconHex->appendPlainText(arrayData);
	}
}

void MainWindow::on_btnTestVideoInformation_clicked()
{
	testPluginVideoInformation(false);
}

void MainWindow::on_btnDebugVideoInformation_clicked()
{
	testPluginVideoInformation(true);
}

void MainWindow::on_btnTestSearchVideos_clicked()
{
	testPluginSearchVideos(false);
}

void MainWindow::on_btnDebugSearchVideos_clicked()
{
	testPluginSearchVideos(true);
}

void MainWindow::on_btnGenerateIcon_clicked()
{
	generateBinaryArray();
}

void MainWindow::on_toolBtnPluginVideoInformation_clicked()
{
	QString pluginFile = QFileDialog::getOpenFileName(this,
		"Open Plugin file", ui->edtPluginPathVideoInformation->text(), "xVST Plugin (*.js)");
	// set file name
	if (!pluginFile.isEmpty())
		ui->edtPluginPathVideoInformation->setText(pluginFile);
}

void MainWindow::on_toolBtnPluginSearchVideos_clicked()
{
	QString pluginFile = QFileDialog::getOpenFileName(this,
		"Open Plugin file", ui->edtPluginPathSearchVideos->text(), "xVST Plugin (*.js)");
	// set file name
	if (!pluginFile.isEmpty())
		ui->edtPluginPathSearchVideos->setText(pluginFile);
}

void MainWindow::on_toolBtnPluginIcon_clicked()
{
	QString imageFile = QFileDialog::getOpenFileName(this,
		"Open Plugin icon", ui->edtImagePath->text(), "Images (*.png *.xpm *.jpg *.jpeg *.gif)");
	// set file name
	if (!imageFile.isEmpty())
		ui->edtImagePath->setText(imageFile);
}
