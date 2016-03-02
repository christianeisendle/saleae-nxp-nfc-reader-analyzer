#ifndef RC663_ANALYZER_H
#define RC663_ANALYZER_H

#include <Analyzer.h>
#include "RC663AnalyzerResults.h"
#include "RC663SimulationDataGenerator.h"

class RC663AnalyzerSettings;
class ANALYZER_EXPORT RC663Analyzer : public Analyzer2
{
public:
	RC663Analyzer();
	virtual ~RC663Analyzer();
	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //functions
	void Setup();
	void AdvanceToActiveEnableEdge();
	bool IsInitialClockPolarityCorrect();
	void AdvanceToActiveEnableEdgeWithCorrectClockPolarity();
	bool WouldAdvancingTheClockToggleEnable();
	void GetWord();

#pragma warning( push )
#pragma warning( disable : 4251 ) //warning C4251: 'SerialAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class
protected:  //vars
	std::auto_ptr< RC663AnalyzerSettings > mSettings;
	std::auto_ptr< RC663AnalyzerResults > mResults;
	bool mSimulationInitilized;
	RC663SimulationDataGenerator mSimulationDataGenerator;

	AnalyzerChannelData* mMosi; 
	AnalyzerChannelData* mMiso;
	AnalyzerChannelData* mClock;
	AnalyzerChannelData* mEnable;

	bool mIsLastFrame;
   bool mIsWriteInstruction;
	U64 mCurrentSample;
	U64 mInstructionCode;
	U64 mInstructionStartSample;
	AnalyzerResults::MarkerType mArrowMarker;
	std::vector<U64> mArrowLocations;

#pragma warning( pop )
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //RC663_ANALYZER_H
