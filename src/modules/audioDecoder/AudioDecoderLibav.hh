/*
 *  AudioDecoderLibav - A libav-based audio decoder
 *  Copyright (C) 2013  Fundació i2CAT, Internet i Innovació digital a Catalunya
 *
 *  This file is part of media-streamer.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors:  Marc Palau <marc.palau@i2cat.net>
 */

#ifndef _AUDIO_DECODER_LIBAV_HH
#define _AUDIO_DECODER_LIBAV_HH

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswresample/swresample.h>
}

#include "../../AudioFrame.hh"
#include "../../FrameQueue.hh"
#include "../../Filter.hh"


class AudioDecoderLibav : public OneToOneFilter {

public:
    AudioDecoderLibav();
    ~AudioDecoderLibav();
    bool configure(SampleFmt sampleFormat, int channels, int sampleRate);
    
protected:
    bool doProcessFrame(Frame *org, Frame *dst);
    FrameQueue* allocQueue(ConnectionData cData);
    bool configure0(SampleFmt sampleFormat, int channels, int sampleRate);

private:
    void initializeEventMap();
    bool resample(AVFrame* src, AudioFrame* dst);
    void checkSampleFormat(int sampleFormat);
    bool inputConfig();
    bool outputConfig();
    bool reconfigureDecoder(AudioFrame* frame);
    bool configEvent(Jzon::Node* params);
    void doGetState(Jzon::Object &filterNode);

    //There is no need of specific reader configuration
    bool specificReaderConfig(int /*readerID*/, FrameQueue* /*queue*/)  {return true;};
    bool specificReaderDelete(int /*readerID*/) {return true;};
    
    //NOTE: There is no need of specific writer configuration
    bool specificWriterConfig(int /*writerID*/) {return true;};
    bool specificWriterDelete(int /*writerID*/) {return true;};
    
    AVCodec             *codec;
    AVCodecContext      *codecCtx;
    AVFrame             *inFrame;
    AVPacket            pkt;
    int                 gotFrame;
    SwrContext          *resampleCtx;
    AVSampleFormat      inLibavSampleFmt;
    AVSampleFormat      outLibavSampleFmt;

    ACodecType fCodec;
    SampleFmt inSampleFmt;
    SampleFmt outSampleFmt;
    unsigned inChannels;
    unsigned outChannels;
    unsigned inSampleRate;
    unsigned outSampleRate;
    unsigned bytesPerSample;
    unsigned char *auxBuff[1];

};

#endif
