#include "Sequencer_ProtocolView.h"


#include <QKeyEvent>
#include <QMessageBox>
#include <QMenu>
#include <QDir>
#include <QFileInfo>
#include <QProgressDialog>
#include <QSettings>

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

void ProtocolView::contextMenu(const QPoint &pos)
{

    try
    {
        QModelIndex index = indexAt(pos);
        if(index.isValid())
        {
            QMenu m(this);
            QAction* grabImagesAction = m.addAction(tr("Grab images"));
            grabImagesAction->setEnabled(false);

            QDir sourceDir("F:/DCIM/100MSDCF");

            ProtocolModel* model = this->getProtocolModel();
            if(!model)
                return;
            Protocol* protocol = model->getProtocol(index);
            if(!protocol)
                return;

            QDir protocolPath = protocol->getProtocolPath(false);
            if(!protocolPath.exists())
                throw AstroBase::DirNotFoundException(protocolPath.absolutePath());

            QString subPath = QString("%0/%1/%2")
                    .arg(protocol->getSubject())
                    .arg(Protocol::typeToString(protocol->getType()))
                    .arg(Protocol::colorChannelToString(protocol->getColorChannel()));

            protocolPath.mkpath(subPath);
            QDir targetDir = protocolPath;
            targetDir.cd(subPath);

            if(!sourceDir.exists())
            {
                grabImagesAction->setToolTip(tr("Source dir not found: %0").arg(sourceDir.absolutePath()));
            }
            else if(!targetDir.exists())
            {
                grabImagesAction->setToolTip(tr("Target dir not found: %0").arg(targetDir.absolutePath()));
            }
            else
            {
                grabImagesAction->setEnabled(true);
                connect(grabImagesAction, &QAction::triggered, [this, sourceDir, targetDir, protocol]()
                {

                    BatchProcess::RawImageGrabber grabber;

                    grabber.setSourceDir(sourceDir);
                    grabber.setTargetDir(targetDir);
                    grabber.setRawFileSuffix("arw");

                    EasyExif::EXIFInfoList input;
                    for(auto ps : protocol->getPhotoShots())
                    {
                        if(ps.exif.isValid())
                            input << EasyExif::EXIFInfoPtr(new EasyExif::EXIFInfo(ps.exif));
                    }
                    grabber.setInput(input);

                    QProgressDialog progress(tr("Grabbing images"), tr("Cancel"), 0, input.count(), this);
                    progress.setAutoClose(true);
                    progress.setMinimumDuration(0);

                    connect(&grabber, &BatchProcess::RawImageGrabber::progress, [&progress](int done, int maxCount)
                    {
                        Q_UNUSED(maxCount);
                        progress.setValue(done);
                    });
                    grabber.process();

                });
            }


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
        questionMsg = tr("Remove protocol \"%0\"?").arg(p->getSubject());
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
