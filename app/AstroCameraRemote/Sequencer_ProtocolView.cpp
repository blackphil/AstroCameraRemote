#include "Sequencer_ProtocolView.h"


#include <QKeyEvent>
#include <QMessageBox>

#include "Sequencer_ProtocolModel.h"

namespace Sequencer {

ProtocolView::ProtocolView(QWidget *parent) :
    QTableView (parent)
{
}

ProtocolView::~ProtocolView()
{
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

} // namespace Sequencer
