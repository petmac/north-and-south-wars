#include "audio.h"

#include "chip.h"

#include "amiga/custom.h"
#include "game/callbacks.h"
#include "game/sounds.h"

#include <hardware/dmabits.h>

// https://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node00DE.html
static constexpr u32 clock_constant_pal = 3546895;
static constexpr u32 sample_rate = 11025;
static constexpr u16 period = clock_constant_pal / sample_rate;

typedef enum ChannelState {
  CS_STOPPED,
  CS_STARTING,
  CS_PLAYING,
} ChannelState;

static volatile ChannelState channels[4];

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

void audioChannelInterruptHandler(u16 channelIndex) {
  volatile ChannelState *const channel_state = &channels[channelIndex];
  volatile Custom::AudChannel *const channel = &custom.aud[channelIndex];

  switch (*channel_state) {
  case CS_STOPPED:
    break;
  case CS_STARTING:
    *channel_state = CS_PLAYING;
    break;
  case CS_PLAYING:
    *channel_state = CS_STOPPED;
    channel->ac_vol = 0;
    custom.dmacon = DMAF_AUD0 << channelIndex;
    break;
  }
}

// Callbacks
void play(Sound sound) {
  const Waves &waves = chip.waves;

  switch (sound) {
  case Sound::cancel:
    play(waves.cancel);
    break;
  case Sound::ok:
    play(waves.ok);
    break;
  }
}
