#include "tester.h"

double 	freq, TotalTime, OverheadTime, TotalRedTime, TotalPatTime, TotalSeqTime;
long 	StartCount, StopCount, StartSeq, StopSeq, StartPatCount, StopPatCount;

void initcntr()
{
	LARGE_INTEGER LI_Freq;
	QueryPerformanceFrequency(&LI_Freq);
	freq = (double)LI_Freq.LowPart;
	startcntr();
	stopcntr();
	OverheadTime = (double)(StopCount - StartCount) / freq;
	output("Overhead Time = %f", OverheadTime);
}

void startcntr()
{
	LARGE_INTEGER LI_Start;
	QueryPerformanceCounter(&LI_Start);
	//	StartCount = (double)LI_Start.LowPart;
	StartCount = LI_Start.LowPart;
}

void startseqcntr()
{
	LARGE_INTEGER LI_Start;
	QueryPerformanceCounter(&LI_Start);
	//	StartSeq = (double)LI_Start.LowPart;
	StartSeq = LI_Start.LowPart;
}

double stopcntr()
{
	LARGE_INTEGER LI_Stop;
	QueryPerformanceCounter(&LI_Stop);
	//	StopCount = (double)LI_Stop.LowPart;
	StopCount = LI_Stop.LowPart;
	TotalTime = (double)(StopCount - StartCount) / freq;
	TotalTime = TotalTime - OverheadTime;
	return(TotalTime);
}

double stopseqcntr()
{
	LARGE_INTEGER LI_Stop;
	QueryPerformanceCounter(&LI_Stop);
	StopSeq = LI_Stop.LowPart;
	TotalSeqTime = (double)(StopSeq - StartSeq) / freq;
	TotalSeqTime = TotalSeqTime - OverheadTime;
	return(TotalSeqTime);
}

void startpattimer()
{
	LARGE_INTEGER LI_Start;
	QueryPerformanceCounter(&LI_Start);
	//	StartPatCount = (double)LI_Start.LowPart;
	StartPatCount = LI_Start.LowPart;
}

double stoppattimer()
{
	LARGE_INTEGER LI_Stop;
	QueryPerformanceCounter(&LI_Stop);
	//	StopPatCount = (double)LI_Stop.LowPart;
	StopPatCount = LI_Stop.LowPart;
	TotalPatTime = (double)(StopPatCount - StartPatCount) / freq;
	TotalPatTime = TotalPatTime - OverheadTime;
	return(TotalPatTime);
}
