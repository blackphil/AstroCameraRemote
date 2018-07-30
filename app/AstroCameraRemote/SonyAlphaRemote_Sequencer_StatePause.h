#ifndef SONYALPHAREMOTE_SEQUENCER_STATEPAUSE_H
#define SONYALPHAREMOTE_SEQUENCER_STATEPAUSE_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"


namespace SonyAlphaRemote {
namespace Sequencer {

class StatePause : public StateBase
{
    Q_OBJECT
public:
    StatePause(QTimer* t);
};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_STATEPAUSE_H
