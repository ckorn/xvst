#include "progressbardelegate.h"

ProgressBarDelegate::ProgressBarDelegate(QObject *parent, VideoListController *videoList) : QItemDelegate(parent)
{
	this->videoList = videoList;
}

void ProgressBarDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option,
                                const QModelIndex & index ) const
{
	if (index.column() == 2)
	{
		// get the video item
		bool converted;
		VideoItem *videoItem = videoList->getVideoItemByID(index.data(Qt::UserRole).toInt(&converted));
		// check if this item exists
		if (videoItem != NULL)
		{
			if (videoItem->isDownloading() || videoItem->isConverting())
			{
				// basic colors
				QColor borderColor(143, 180, 219);
				QColor backgroundColor(198, 209, 221);
				// downloading colors
				QColor dwn_progressBorderColor(35, 96, 167);
				QColor dwn_progressColor1(100, 136, 252);
				QColor dwn_progressColor2(165, 183, 240);//135, 160, 240);
				// converting colors
				QColor cnv_progressBorderColor(209, 128, 24);
				QColor cnv_progressColor1(246, 199, 138);
				QColor cnv_progressColor2(255, 227, 190);

				// paint the progressBar background
				painter->setBrush(backgroundColor);
				painter->setPen(borderColor);
				painter->drawRect(option.rect.x(), option.rect.y(),
				                  option.rect.width() - 1, option.rect.height() - 1);

				// define gradient
				QLinearGradient linearGrad(option.rect.x(), option.rect.y(),
				                           option.rect.x(), option.rect.y() + option.rect.height() - 1);

				// define gradient colors
				if (videoItem->isDownloading()) // downloading colors schema
				{
					linearGrad.setColorAt(0.00, dwn_progressColor1);
					linearGrad.setColorAt(0.16, dwn_progressColor2);
					linearGrad.setColorAt(1.00, dwn_progressColor1);
					painter->setPen(dwn_progressBorderColor);
				}
				else // converting colors schema
				{
					linearGrad.setColorAt(0.00, cnv_progressColor1);
					linearGrad.setColorAt(0.16, cnv_progressColor2);
					linearGrad.setColorAt(1.00, cnv_progressColor1);
					painter->setPen(cnv_progressBorderColor);
				}

				// calculre progress value
				int preWidth = static_cast<int>((option.rect.width() - 1)*(videoItem->getProgress()/100));
				int progressWidth = option.rect.width() - preWidth;
				if (progressWidth == option.rect.width()) return;

				// paint the progress
				painter->setBrush(linearGrad);
				painter->drawRect(option.rect.x(), option.rect.y(),
				                  option.rect.width() - progressWidth, option.rect.height() - 1);
				// ok
				return;
			}
		}
	}
	// default painting
	QItemDelegate::paint(painter, option, index);
	return;
}
