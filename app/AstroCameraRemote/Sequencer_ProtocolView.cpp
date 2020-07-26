#include "Sequencer_ProtocolView.h"


#include <QKeyEvent>
#include <QMessageBox>
#include <QMenu>
#include <QDir>
#include <QFileInfo>
#include <QProgressDialog>
#include <QSettings>
#include <QFileDialog>

#include "EasyExif_Exif.h"
#include "AstroBase.h"
#include "AstroBase_Exception.h"
#include "Sequencer_ProtocolModel.h"
#include "BatchProcess_RawImageGrabber.h"

namespace Sequencer {

ProtocolView::ProtocolView(QWidget *parent)
    : QTableView (parent)
    , msgPoster(new MessagePoster(this))
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(activateSelectedProtocol()));
}

ProtocolView::~ProtocolView()
{
}

Protocol *ProtocolView::getSelectedProtocol() const
{
    QModelIndexList selection = selectionModel()->selectedIndexes();
    if(selection.isEmpty())
        return nullptr;

    QModelIndex selIndex = selection.first();
    if(!selIndex.isValid())
        return nullptr;

    ProtocolModel* pModel = getProtocolModel();
    if(!pModel)
        return nullptr;

    return pModel->getProtocol(selIndex);
}

QString ProtocolView::getSourceDir()
{
    constexpr auto settingsKey = "ImageGrabber/LastSourceDirs";
    auto history = QSettings().value(settingsKey).toStringList();

    QFileDialog fileDialog(this, tr("Grab images from ..."), history.isEmpty() ? QDir::homePath() : history.first());
    fileDialog.setOption(QFileDialog::Option::ShowDirsOnly, true);
    fileDialog.setFileMode(QFileDialog::FileMode::DirectoryOnly);
    fileDialog.setHistory(history);

    if(fileDialog.exec())
    {
        if(auto selection = fileDialog.selectedFiles(); !selection.isEmpty())
        {
            if(auto srcDir = selection.first(); !srcDir.isEmpty())
            {
                history.removeAll(srcDir);
                history.prepend(srcDir);
                QSettings().setValue(settingsKey, history);
                return srcDir;
            }
        }
    }

    return {};
}

void ProtocolView::contextMenu(const QPoint &pos)
{

    try
    {
        if(QModelIndex index { indexAt(pos) }; index.isValid())
        {
            QMenu m(this);

            ProtocolModel* model = this->getProtocolModel();
            if(!model)
                return;
            Protocol* protocol = model->getProtocol(index);
            if(!protocol)
                return;

            QAction* activateProtocol = m.addAction(tr("Activate"));
            connect(activateProtocol, &QAction::triggered, [this]()
            {
                Q_EMIT activateSelectedProtocol();
            });

            QAction* grabImagesAction = m.addAction(tr("Grab images"));

            connect(grabImagesAction, &QAction::triggered, [this, protocol]()
            {
                QDir protocolPath = protocol->getProtocolPath(false);
                if(!protocolPath.exists())
                    throw AstroBase::DirNotFoundException(protocolPath.absolutePath());

                if(QString srcDirStr = getSourceDir(); !srcDirStr.isEmpty())
                {
                    QDir sourceDir{srcDirStr};
                    if(!sourceDir.exists())
                    {
                        //ERRROR
                        return;
                    }

                    const Protocol::PhotoShotMap& map = protocol->getPhotoShots();

                    int allCount{0};
                    for(const auto& entry: map)
                    {
                        allCount += entry.count();
                    }


                    QProgressDialog progress(tr("Grabbing images"), tr("Cancel"), 0, allCount, this);
                    progress.setAutoClose(true);
                    progress.setMinimumDuration(0);

                    int index{0};

                    for(Protocol::PhotoShotMap::const_iterator it=map.begin(); it!=map.end(); ++it)
                    {
                        PhotoShot::Type type = it.key();


                        BatchProcess::RawImageGrabber grabber;

                        grabber.setSourceDir(sourceDir);

                        QString subPath = QString("%0/%1")
                                .arg(protocol->getObjectName())
                                .arg(PhotoShot::typeToString(type));

                        protocolPath.mkpath(subPath);
                        QDir targetDir = protocolPath;
                        targetDir.cd(subPath);

                        grabber.setTargetDir(targetDir);
                        grabber.setRawFileSuffix("arw");

                        EasyExif::EXIFInfoList input;

                        for(auto ps : it.value())
                        {
                            if(ps.exif.isValid())
                                input << EasyExif::EXIFInfoPtr(new EasyExif::EXIFInfo(ps.exif));

                        }

                        grabber.setInput(input);
                        connect(&grabber, &BatchProcess::RawImageGrabber::progress, [&progress, &index](int, int)
                        {
                            progress.setValue(index++);
                            qApp->processEvents(/*QEventLoop::ProcessEventsFlag::ExcludeUserInputEvents*/);
                        });
                        grabber.process();
                    }
                }
            });

            m.exec(mapToGlobal(pos));
        }
    }
    catch(AstroBase::Exception& e)
    {
        Q_EMIT msgPoster->error(tr("Grabbing imaged failed: %0").arg(e.what()));
    }
}

void ProtocolView::keyPressEvent(QKeyEvent *ke)
{
    QModelIndexList selectedIndexes = selectionModel()->selectedRows();
    if(selectedIndexes.isEmpty())
        return;

    ProtocolModel* pModel = qobject_cast<ProtocolModel*>(model());
    Q_ASSERT(pModel);
    if(!pModel)
        return;

    QString questionMsg;
    if(selectedIndexes.count() == 1)
    {
        Protocol* p = pModel->getProtocol(selectedIndexes.first());
        Q_ASSERT(p);
        if(!p)
            return;
        questionMsg = tr("Remove protocol \"%0\"?").arg(p->getObjectName());
    }
    else
    {
        questionMsg = tr("Remove %0 protocols?").arg(selectedIndexes.count());
    }

    if(ke->key() == Qt::Key_Delete)
    {
        if(QMessageBox::Yes == QMessageBox::question(this, tr("Remove protocol(s)"), questionMsg))
        {
            for(QModelIndex index : selectedIndexes)
            {
                model()->removeRow(index.row());
            }
        }
    }
}

ProtocolModel* ProtocolView::getProtocolModel() const
{
    ProtocolModel* pModel = qobject_cast<ProtocolModel*>(model());
    Q_ASSERT(pModel);
    return pModel;
}



} // namespace Sequencer
