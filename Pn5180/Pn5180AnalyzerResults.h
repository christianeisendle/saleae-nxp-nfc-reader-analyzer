#ifndef Pn5180_ANALYZER_RESULTS
#define Pn5180_ANALYZER_RESULTS

#include <AnalyzerResults.h>

#define Pn5180_ERROR_FLAG ( 1 << 0 )
#define Pn5180_NEW_FRAME_FLAG ( 1 << 1 )
#define Pn5180_INSTRUCTION_FLAG ( 1 << 2 )

class Pn5180Analyzer;
class Pn5180AnalyzerSettings;

class Pn5180AnalyzerResults : public AnalyzerResults
{
public:
	Pn5180AnalyzerResults( Pn5180Analyzer* analyzer, Pn5180AnalyzerSettings* settings );
	virtual ~Pn5180AnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	Pn5180AnalyzerSettings* mSettings;
	Pn5180Analyzer* mAnalyzer;
	U32 mCnt;
	U64 mOldFrameId;
};

#endif //Pn5180_ANALYZER_RESULTS
