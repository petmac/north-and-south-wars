#include "audio.h"

#include "chip.h"

#include "amiga/custom.h"
#include "game/callbacks.h"
#include "game/sounds.h"

#include <devices/audio.h>
#include <hardware/dmabits.h>
#include <proto/exec.h> // Bartman's compiler needs this

// https://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node00DE.html
static constexpr u32 clock_constant_pal = 3546895;
static constexpr u32 sample_rate = 11025;
static constexpr u16 period =
    (clock_constant_pal + (sample_rate / 2)) / sample_rate;

typedef enum ChannelState {
  CS_STOPPED,
  CS_STARTING,
  CS_PLAYING,
} ChannelState;

static IOAudio s_sIoAudio;
static volatile ChannelState channels[4];

// Messageport creation for KS1.3
// http://amigadev.elowar.com/read/ADCD_2.1/Libraries_Manual_guide/node02EC.html
static struct MsgPort *msgPortCreate(const char *name, LONG pri) {
  LONG sigBit;
  struct MsgPort *mp;

  if ((sigBit = AllocSignal(-1L)) == -1) {
    return NULL;
  }

  mp = (struct MsgPort *)AllocMem(sizeof(*mp), MEMF_PUBLIC | MEMF_CLEAR);
  if (!mp) {
    FreeSignal(sigBit);
    return NULL;
  }
  mp->mp_Node.ln_Name = const_cast<char *>(name);
  mp->mp_Node.ln_Pri = pri;
  mp->mp_Node.ln_Type = NT_MSGPORT;
  mp->mp_Flags = PA_SIGNAL;
  mp->mp_SigBit = sigBit;
  mp->mp_SigTask = FindTask(NULL);

  // NewList(&(mp->mp_MsgList)); // Init message list - not in headers
  mp->mp_MsgList.lh_Head = (struct Node *)&mp->mp_MsgList.lh_Tail;
  mp->mp_MsgList.lh_Tail = NULL;
  mp->mp_MsgList.lh_TailPred = (struct Node *)&mp->mp_MsgList.lh_Head;
  return mp;
}

static void msgPortDelete(struct MsgPort *mp) {
  mp->mp_SigTask = (struct Task *)-1; // Make it difficult to re-use the port
  mp->mp_MsgList.lh_Head = (struct Node *)-1;

  FreeSignal(mp->mp_SigBit);
  FreeMem(mp, sizeof(*mp));
}

template <u16 length>
static void play_sound_on_channel(u16 channel_index, const Wave<length> &wave) {
  volatile ChannelState *const channel_state = &channels[channel_index];
  volatile Custom::AudChannel *const channel = &custom.aud[channel_index];
  const u16 dma_flag = DMAF_AUD0 << channel_index;

  *channel_state = CS_STARTING;

  channel->ac_ptr = (u16 *)&wave.data[0];
  channel->ac_len = length >> 1;
  channel->ac_per = period;
  channel->ac_vol = 64;

  custom.dmacon = DMAF_SETCLR | dma_flag;
}

template <u16 length> static void play(const Wave<length> &wave) {
  for (u16 channel_index = 0; channel_index < 4; ++channel_index) {
    if (channels[channel_index] == CS_STOPPED) {
      play_sound_on_channel(channel_index, wave);
      return;
    }
  }

  for (u16 channel_index = 0; channel_index < 4; ++channel_index) {
    if (channels[channel_index] == CS_PLAYING) {
      play_sound_on_channel(channel_index, wave);
      return;
    }
  }
}

static void ioRequestInitialize(struct IORequest *pIoReq,
                                struct MsgPort *pMsgPort, UWORD uwLen) {
  pIoReq->io_Message.mn_Node.ln_Type = NT_MESSAGE;
  pIoReq->io_Message.mn_ReplyPort = pMsgPort;
  pIoReq->io_Message.mn_Length = uwLen;
}

static struct IORequest *ioRequestCreate(struct MsgPort *pMsgPort,
                                         UWORD uwLen) {
  struct IORequest *pIoReq =
      (struct IORequest *)AllocMem(uwLen, MEMF_PUBLIC | MEMF_CLEAR);
  if (!pIoReq) {
    return NULL;
  }
  ioRequestInitialize(pIoReq, pMsgPort, uwLen);
  return pIoReq;
}

static void ioRequestDestroy(struct IORequest *pIoReq) {
  FreeMem(pIoReq, pIoReq->io_Message.mn_Length);
}

bool audioChannelAlloc() {
  struct MsgPort *pMsgPort = msgPortCreate("audio alloc", ADALLOC_MAXPREC);
  if (!pMsgPort) {
    KPrintF("ERR: Couldn't open message port for audio alloc\n");
    return false;
  }

  // memset(&s_sIoAudio, 0, sizeof(s_sIoAudio));
  ioRequestInitialize(&s_sIoAudio.ioa_Request, pMsgPort, sizeof(s_sIoAudio));

  UBYTE isError = OpenDevice((CONST_STRPTR) "audio.device", 0,
                             (struct IORequest *)&s_sIoAudio, 0);
  if (isError) {
    KPrintF("ERR: Couldn't alloc Audio channels, code: %d\n",
            s_sIoAudio.ioa_Request.io_Error);
    msgPortDelete(pMsgPort);
    return false;
  }

  UBYTE ubChannelMask = 0b1111; // Allocate all 4 channels.
  s_sIoAudio.ioa_Data = &ubChannelMask;
  s_sIoAudio.ioa_Length = sizeof(ubChannelMask);
  s_sIoAudio.ioa_Request.io_Command = ADCMD_ALLOCATE;
  s_sIoAudio.ioa_Request.io_Flags = ADIOF_NOWAIT;
  isError = DoIO((struct IORequest *)&s_sIoAudio);

  if (isError) {
    KPrintF("ERR: io audio request fail, code: %d\n",
            s_sIoAudio.ioa_Request.io_Error);
    msgPortDelete(pMsgPort);
    return false;
  }

  return true;
}

void audioChannelFree() {
  struct MsgPort *pMsgPort = s_sIoAudio.ioa_Request.io_Message.mn_ReplyPort;

  s_sIoAudio.ioa_Request.io_Command = ADCMD_FINISH;
  s_sIoAudio.ioa_Request.io_Flags = 0;
  s_sIoAudio.ioa_Request.io_Unit = reinterpret_cast<Unit *>(1);
  UBYTE isError = DoIO((struct IORequest *)&s_sIoAudio);
  if (isError) {
    KPrintF("ERR: io audio request fail, code: %d\n",
            s_sIoAudio.ioa_Request.io_Error);
  }

  CloseDevice((struct IORequest *)&s_sIoAudio);
  msgPortDelete(pMsgPort);
}

void audioChannelInterruptHandler(u16 channelIndex) {
  volatile ChannelState *const channel_state = &channels[channelIndex];
  volatile Custom::AudChannel *const channel = &custom.aud[channelIndex];

  switch (*channel_state) {
  case CS_STARTING:
    *channel_state = CS_PLAYING;
    break;
  case CS_PLAYING:
  case CS_STOPPED:
    custom.dmacon = DMAF_AUD0 << channelIndex;
    channel->ac_vol = 0;
    *channel_state = CS_STOPPED;
    break;
  }
}

// Callbacks
void play(Sound sound) {
  switch (sound) {
  case Sound::cancel:
    play(chip.main.cancel);
    break;
  case Sound::footsteps:
    play(chip.mission.footsteps);
    break;
  case Sound::infantryAttack:
    play(chip.mission.infantryAttack);
    break;
  case Sound::ok:
    play(chip.main.ok);
    break;
  case Sound::zoomIn:
    play(chip.mission.zoomIn);
    break;
  case Sound::zoomOut:
    play(chip.mission.zoomOut);
    break;
  }
}
