/*
 *  AudioCircularBuffer - Audio circular buffer
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

#ifndef _AUDIO_CIRCULAR_BUFFER_HH
#define _AUDIO_CIRCULAR_BUFFER_HH

#include "Types.hh"
#include "FrameQueue.hh"
#include "AudioFrame.hh"
#include <mutex>

#define DEFAULT_BUFFER_SIZE 32768 //samples (~600ms at 48KHz)


 class AudioCircularBuffer : public FrameQueue {

public:
    static AudioCircularBuffer* createNew(struct ConnectionData cData, unsigned ch, unsigned sRate, unsigned maxSamples, SampleFmt sFmt);
    ~AudioCircularBuffer();
    void setOutputFrameSamples(int samples); 

    /**
    * See FrameQueue::getRear
    */
    Frame *getRear();
    
    /**
    * See FrameQueue::getFront
    */
    Frame *getFront();
    
    /**
    * See FrameQueue::addFrame
    */
    std::vector<int> addFrame();
    
    /**
    * See FrameQueue::removeFrame
    */
    int removeFrame();
    
    
    void doFlush();
    
    /**
    * See FrameQueue::forceGetRear
    */
    Frame *forceGetRear();
    
    /**
    * See FrameQueue::forceGetFront
    */
    Frame *forceGetFront();

    /**
     * get the number of free samples in the current buffer
     * @return number of free samples.
     */
    int getFreeSamples();
    
    /**
     * returns the maximum number of samples per channel.
     * @return max number of samples.
     */
    unsigned getChannelMaxSamples() {return chMaxSamples;};
    
    /**
    * See FrameQueue::getElements
    */
    unsigned getElements() const;
    
    /**
    * Tests if the current queue is full or not
    * @return true if the number of elements exceeds the threshold level
    */
    bool isFull() const;

private:
    AudioCircularBuffer(struct ConnectionData cData, unsigned ch, unsigned sRate, unsigned maxSamples, SampleFmt sFmt);

    bool pushBack(unsigned char **buffer, int samplesRequested);
    bool forcePushBack(unsigned char **buffer, int samplesRequested);
    bool popFront(unsigned char **buffer, unsigned samplesRequested);
    void fillOutputBuffers(unsigned char **buffer, int bytesRequested);
    bool setup();

    unsigned channels;
    unsigned sampleRate;
    unsigned bytesPerSample;
    unsigned chMaxSamples;
    unsigned channelMaxLength;
    unsigned char *data[MAX_CHANNELS];
    SampleFmt sampleFormat;
    bool fillNewFrame;

    PlanarAudioFrame* inputFrame;
    PlanarAudioFrame* outputFrame;
    PlanarAudioFrame* dummyFrame;

    std::chrono::microseconds syncTimestamp;
    bool synchronized;
    bool setupSuccess;
    
    std::chrono::system_clock::time_point orgTime;

    int tsDeviationThreshold;
    std::mutex mtx;

    unsigned elements;
};

#endif
