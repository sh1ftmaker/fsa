#ifndef JAUDIO_JASFAKEMATCH10_H
#define JAUDIO_JASFAKEMATCH10_H

#ifdef MATCHING

#include "JSystem/JAudio/JASFakeMatch.h"

template <int N>
class JAUAudible;
class JAIStream;
class JAISeq;
class JAISe;
class JAISoundChild;

typedef JAUDopplerAudible<4> TDopplerAudible4;

JASMemPool<TDopplerAudible4> JASPoolAllocObject<TDopplerAudible4>::memPool_;
JASMemPool<JAIStream> JASPoolAllocObject<JAIStream>::memPool_;
JASMemPool<JAISoundChild> JASPoolAllocObject<JAISoundChild>::memPool_;
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif
