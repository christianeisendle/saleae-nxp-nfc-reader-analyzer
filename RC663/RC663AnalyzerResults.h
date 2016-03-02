#ifndef RC663_ANALYZER_RESULTS
#define RC663_ANALYZER_RESULTS

#include <AnalyzerResults.h>

#define RC663_ERROR_FLAG ( 1 << 0 )
#define RC663_NEW_FRAME_FLAG ( 1 << 1 )
#define RC663_REGISTER_FLAG ( 1 << 2 )
#define RC663_WRITE_PROCESS_FLAG ( 1 << 3)

class RC663Analyzer;
class RC663AnalyzerSettings;

class RC663AnalyzerResults : public AnalyzerResults
{
   public:
	   RC663AnalyzerResults( RC663Analyzer* analyzer, RC663AnalyzerSettings* settings );
	   virtual ~RC663AnalyzerResults();

	   virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	   virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	   virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	   virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	   virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

   protected: //functions

   protected: //vars
	   RC663AnalyzerSettings* mSettings;
	   RC663Analyzer* mAnalyzer;
	   U32 mCnt;
	   U64 mOldFrameId;

   private:
      void ExportSignalsToFile( const char* file, DisplayBase display_base );
      void ExportRegistersToFile( const char* file, DisplayBase display_base );
      void ExportFramesToFile( const char* file, DisplayBase display_base );

      // helper function
      void PrintEntryOfExportRegister(void* f, DisplayBase display_base, RC663Analyzer *analyzer, S64 startingSampleInclusive, S64 endingSampleInclusive, S64 prevEndingSampleInclusive, U64 reg, bool isWriteProcess, U64 *payload, U64 lenPayload );
      void replaceDotsWithComma( char* text );
};

#endif //RC663_ANALYZER_RESULTS
