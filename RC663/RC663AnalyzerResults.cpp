#include "RC663AnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "RC663Analyzer.h"
#include "RC663AnalyzerSettings.h"
#include <iostream>
#include <sstream>

#pragma warning(disable: 4996) //warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.

struct TRC663RegisterNames
{
   const char* long_text;
   const char* short_text;
};

#define NUM_REGISTERS 128U

// following reg-expressions can be used to delete all lines in between the defines when copying from "phhalHw_RC663_Reg.h"
//  a) delete all comments>     /\*[ @A-Za-z0-9_,()\\\.*\n\-\}/\+]*\*/
//  b) delete all empty lines>  ^$\n
//  c) reformat remaining lines>   find-box = \#define {[A-Za-z0-9_]+}{ +}    replace-box=    "\1",\2// 
//
const TRC663RegisterNames RC663Registers[NUM_REGISTERS] = 
{
   {"PHHAL_HW_RC663_REG_COMMAND",              "REG_COMMAND"},              // 0x00
   {"PHHAL_HW_RC663_REG_HOSTCTRL",             "REG_HOSTCTRL"},             // 0x01
   {"PHHAL_HW_RC663_REG_FIFOCONTROL",          "REG_FIFOCONTROL"},          // 0x02
   {"PHHAL_HW_RC663_REG_WATERLEVEL",           "REG_WATERLEVEL"},           // 0x03
   {"PHHAL_HW_RC663_REG_FIFOLENGTH",           "REG_FIFOLENGTH"},           // 0x04
   {"PHHAL_HW_RC663_REG_FIFODATA",             "REG_FIFODATA"},             // 0x05
   {"PHHAL_HW_RC663_REG_IRQ0",                 "REG_IRQ0"},                 // 0x06
   {"PHHAL_HW_RC663_REG_IRQ1",                 "REG_IRQ1"},                 // 0x07
   {"PHHAL_HW_RC663_REG_IRQ0EN",               "REG_IRQ0EN"},               // 0x08
   {"PHHAL_HW_RC663_REG_IRQ1EN",               "REG_IRQ1EN"},               // 0x09
   {"PHHAL_HW_RC663_REG_ERROR",                "REG_ERROR"},                // 0x0A
   {"PHHAL_HW_RC663_REG_STATUS",               "REG_STATUS"},               // 0x0B
   {"PHHAL_HW_RC663_REG_RXBITCTRL",            "REG_RXBITCTRL"},            // 0x0C
   {"PHHAL_HW_RC663_REG_RXCOLL",               "REG_RXCOLL"},               // 0x0D
   {"PHHAL_HW_RC663_REG_TCONTROL",             "REG_TCONTROL"},             // 0x0E
   {"PHHAL_HW_RC663_REG_T0CONTROL",            "REG_T0CONTROL"},            // 0x0F
   {"PHHAL_HW_RC663_REG_T0RELOADHI",           "REG_T0RELOADHI"},           // 0x10
   {"PHHAL_HW_RC663_REG_T0RELOADLO",           "REG_T0RELOADLO"},           // 0x11
   {"PHHAL_HW_RC663_REG_T0COUNTERVALHI",       "REG_T0COUNTERVALHI"},       // 0x12
   {"PHHAL_HW_RC663_REG_T0COUNTERVALLO",       "REG_T0COUNTERVALLO"},       // 0x13
   {"PHHAL_HW_RC663_REG_T1CONTROL",            "REG_T1CONTROL"},            // 0x14
   {"PHHAL_HW_RC663_REG_T1RELOADHI",           "REG_T1RELOADHI"},           // 0x15
   {"PHHAL_HW_RC663_REG_T1RELOADLO",           "REG_T1RELOADLO"},           // 0x16
   {"PHHAL_HW_RC663_REG_T1COUNTERVALHI",       "REG_T1COUNTERVALHI"},       // 0x17
   {"PHHAL_HW_RC663_REG_T1COUNTERVALLO",       "REG_T1COUNTERVALLO"},       // 0x18
   {"PHHAL_HW_RC663_REG_T2CONTROL",            "REG_T2CONTROL"},            // 0x19
   {"PHHAL_HW_RC663_REG_T2RELOADHI",           "REG_T2RELOADHI"},           // 0x1A
   {"PHHAL_HW_RC663_REG_T2RELOADLO",           "REG_T2RELOADLO"},           // 0x1B
   {"PHHAL_HW_RC663_REG_T2COUNTERVALHI",       "REG_T2COUNTERVALHI"},       // 0x1C
   {"PHHAL_HW_RC663_REG_T2COUNTERVALLO",       "REG_T2COUNTERVALLO"},       // 0x1D
   {"PHHAL_HW_RC663_REG_T3CONTROL",            "REG_T3CONTROL"},            // 0x1E
   {"PHHAL_HW_RC663_REG_T3RELOADHI",           "REG_T3RELOADHI"},           // 0x1F
   {"PHHAL_HW_RC663_REG_T3RELOADLO",           "REG_T3RELOADLO"},           // 0x20
   {"PHHAL_HW_RC663_REG_T3COUNTERVALHI",       "REG_T3COUNTERVALHI"},       // 0x21
   {"PHHAL_HW_RC663_REG_T3COUNTERVALLO",       "REG_T3COUNTERVALLO"},       // 0x22
   {"PHHAL_HW_RC663_REG_T4CONTROL",            "REG_T4CONTROL"},            // 0x23
   {"PHHAL_HW_RC663_REG_T4RELOADHI",           "REG_T4RELOADHI"},           // 0x24
   {"PHHAL_HW_RC663_REG_T4RELOADLO",           "REG_T4RELOADLO"},           // 0x25
   {"PHHAL_HW_RC663_REG_T4COUNTERVALHI",       "REG_T4COUNTERVALHI"},       // 0x26
   {"PHHAL_HW_RC663_REG_T4COUNTERVALLO",       "REG_T4COUNTERVALLO"},       // 0x27
   {"PHHAL_HW_RC663_REG_DRVMODE",              "REG_DRVMODE"},              // 0x28
   {"PHHAL_HW_RC663_REG_TXAMP",                "REG_TXAMP"},                // 0x29
   {"PHHAL_HW_RC663_REG_DRVCON",               "REG_DRVCON"},               // 0x2A
   {"PHHAL_HW_RC663_REG_TXI",                  "REG_TXI"},                  // 0x2B
   {"PHHAL_HW_RC663_REG_TXCRCCON",             "REG_TXCRCCON"},             // 0x2C
   {"PHHAL_HW_RC663_REG_RXCRCCON",             "REG_RXCRCCON"},             // 0x2D
   {"PHHAL_HW_RC663_REG_TXDATANUM",            "REG_TXDATANUM"},            // 0x2E
   {"PHHAL_HW_RC663_REG_TXMODWIDTH",           "REG_TXMODWIDTH"},           // 0x2F
   {"PHHAL_HW_RC663_REG_TXSYM10BURSTLEN",      "REG_TXSYM10BURSTLEN"},      // 0x30
   {"PHHAL_HW_RC663_REG_TXWAITCTRL",           "REG_TXWAITCTRL"},           // 0x31
   {"PHHAL_HW_RC663_REG_TXWAITLO",             "REG_TXWAITLO"},             // 0x32
   {"PHHAL_HW_RC663_REG_FRAMECON",             "REG_FRAMECON"},             // 0x33
   {"PHHAL_HW_RC663_REG_RXSOFD",               "REG_RXSOFD"},               // 0x34
   {"PHHAL_HW_RC663_REG_RXCTRL",               "REG_RXCTRL"},               // 0x35
   {"PHHAL_HW_RC663_REG_RXWAIT",               "REG_RXWAIT"},               // 0x36
   {"PHHAL_HW_RC663_REG_RXTHRESHOLD",          "REG_RXTHRESHOLD"},          // 0x37
   {"PHHAL_HW_RC663_REG_RCV",                  "REG_RCV"},                  // 0x38
   {"PHHAL_HW_RC663_REG_RXANA",                "REG_RXANA"},                // 0x39
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x3A
   {"PHHAL_HW_RC663_REG_SERIALSPEED",          "REG_SERIALSPEED"},          // 0x3B
   {"PHHAL_HW_RC663_REG_LPO_TRIMM",            "REG_LPO_TRIMM"},            // 0x3C
   {"PHHAL_HW_RC663_REG_PLL_CTRL",             "REG_PLL_CTRL"},             // 0x3D
   {"PHHAL_HW_RC663_REG_PLL_DIV",              "REG_PLL_DIV"},              // 0x3E
   {"PHHAL_HW_RC663_REG_LPCD_QMIN",            "REG_LPCD_QMIN"},            // 0x3F
   {"PHHAL_HW_RC663_REG_LPCD_QMAX",            "REG_LPCD_QMAX"},            // 0x40
   {"PHHAL_HW_RC663_REG_LPCD_IMIN",            "REG_LPCD_IMIN"},            // 0x41
   {"PHHAL_HW_RC663_REG_LPCD_RESULT_I",        "REG_LPCD_RESULT_I"},        // 0x42
   {"PHHAL_HW_RC663_REG_LPCD_RESULT_Q",        "REG_LPCD_RESULT_Q"},        // 0x43
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x44
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x45
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x46
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x47
   {"PHHAL_HW_RC663_TXBITMOD",                 "TXBITMOD"},                 // 0x48
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x49
   {"PHHAL_HW_RC663_REG_TXDATACON",            "REG_TXDATACON"},            // 0x4A
   {"PHHAL_HW_RC663_REG_TXDATAMOD",            "REG_TXDATAMOD"},            // 0x4B
   {"PHHAL_HW_RC663_REG_TXSYMFREQ",            "REG_TXSYMFREQ"},            // 0x4C
   {"PHHAL_HW_RC663_REG_TXSYM0H",              "REG_TXSYM0H"},              // 0x4D
   {"PHHAL_HW_RC663_REG_TXSYM0L",              "REG_TXSYM0L"},              // 0x4E
   {"PHHAL_HW_RC663_REG_TXSYM1H",              "REG_TXSYM1H"},              // 0x4F
   {"PHHAL_HW_RC663_REG_TXSYM1L",              "REG_TXSYM1L"},              // 0x50
   {"PHHAL_HW_RC663_REG_TXSYM2",               "REG_TXSYM2"},               // 0x51
   {"PHHAL_HW_RC663_REG_TXSYM3",               "REG_TXSYM3"},               // 0x52
   {"PHHAL_HW_RC663_REG_TXSYM10LEN",           "REG_TXSYM10LEN"},           // 0x53
   {"PHHAL_HW_RC663_REG_TXSYM32LEN",           "REG_TXSYM32LEN"},           // 0x54
   {"PHHAL_HW_RC663_REG_TXSYM10BURSTCTRL",     "REG_TXSYM10BURSTCTRL"},     // 0x55
   {"PHHAL_HW_RC663_REG_TXSYM10MOD",           "REG_TXSYM10MOD"},           // 0x56
   {"PHHAL_HW_RC663_REG_TXSYM32MOD",           "REG_TXSYM32MOD"},           // 0x57
   {"PHHAL_HW_RC663_REG_RXBITMOD",             "REG_RXBITMOD"},             // 0x58
   {"PHHAL_HW_RC663_REG_RXEOFSYM",             "REG_RXEOFSYM"},             // 0x59
   {"PHHAL_HW_RC663_REG_RXSYNCVAlH",           "REG_RXSYNCVAlH"},           // 0x5A
   {"PHHAL_HW_RC663_REG_RXSYNCVAlL",           "REG_RXSYNCVAlL"},           // 0x5B
   {"PHHAL_HW_RC663_REG_RXSYNCMOD",            "REG_RXSYNCMOD"},            // 0x5C
   {"PHHAL_HW_RC663_REG_RXMOD",                "REG_RXMOD"},                // 0x5D
   {"PHHAL_HW_RC663_REG_RXCORR",               "REG_RXCORR"},               // 0x5E
   {"PHHAL_HW_RC663_REG_RXSVETTE",             "REG_RXSVETTE"},             // 0x5F
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x60
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x61
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x62
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x63
   {"PHHAL_HW_RC663_REG_DACVAL",               "REG_DACVAL"},               // 0x64
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x65
   {"PHHAL_HW_RC663_REG_TESTMOD",              "REG_TESTMOD"},              // 0x66
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x67
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x68
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x69
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x6A
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x6B
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x6C
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x6D
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x6E
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x6F
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x70
   {"PHHAL_HW_RC663_REG_ANAXTAL",              "REG_ANAXTAL"},              // 0x71
   {"PHHAL_HW_RC663_REG_SIGPROTEST",           "REG_SIGPROTEST"},           // 0x72
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x73
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x74
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x75
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x76
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x77
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x78
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x79
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x7A
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x7B
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x7C
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x7D
   {"-- unknown reg. --",                        "-- unknown reg. --"},       // 0x7E
   {"PHHAL_HW_RC663_REG_VERSION",              "REG_VERSION"}                // 0x7F
};

/*****************************************************************************/

RC663AnalyzerResults::RC663AnalyzerResults( RC663Analyzer* analyzer, RC663AnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer ),
	mCnt( 0 ),
	mOldFrameId( 0 )
{
}

/*****************************************************************************/

RC663AnalyzerResults::~RC663AnalyzerResults()
{
}

/*****************************************************************************/

void RC663AnalyzerResults::replaceDotsWithComma( char* text )
{
   char* pch;

   pch = strchr(text, '.');
   if(pch) *pch = ',';
}

/*****************************************************************************/

void RC663AnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();

	Frame frame = GetFrame( frame_index );

	if( frame.mFlags & RC663_REGISTER_FLAG )
	{
		if ( channel == mSettings->mEnableChannel )
		{
			U64 regDef = frame.mData1 >> 1;

			if (regDef >= NUM_REGISTERS)
			{
				char number_str[128];
				AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
				AddResultString( number_str );
				AddResultString( "Unknown Instruction" );
			}
			else
			{
            if(frame.mData1 & 0x01)
            {
   				AddResultString( "read" );
               AddResultString( "read ", RC663Registers[regDef].short_text );
               AddResultString( "read ", RC663Registers[regDef].long_text);
            }
            else
            {
   				AddResultString( "write" );
               AddResultString( "write ", RC663Registers[regDef].short_text );
               AddResultString( "write ", RC663Registers[regDef].long_text );
            }
			}
		}
	}
	else
	{
		if( ( frame.mFlags & RC663_ERROR_FLAG ) == 0 )
		{
			if( channel == mSettings->mMosiChannel )
			{
				char number_str[128];
				AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
				AddResultString( number_str );
			}else
			{
				char number_str[128];
				AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, number_str, 128 );
				AddResultString( number_str );
			}
		}else
		{
			AddResultString( "Error" );
			AddResultString( "Settings mismatch" );
			AddResultString( "The initial (idle) state of the CLK line does not match the settings." );
		}
	}
}

/*****************************************************************************/

void RC663AnalyzerResults::ExportSignalsToFile( const char* file, DisplayBase display_base )
{
	//export_type_user_id is only important if we have more than one export type.
	std::stringstream ss;
	void* f = AnalyzerHelpers::StartFile( file );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	ss << "Time [s],Packet ID,MOSI,MISO" << std::endl;

	bool mosi_used = true;
	bool miso_used = true;

	if( mSettings->mMosiChannel == UNDEFINED_CHANNEL )
		mosi_used = false;

	if( mSettings->mMisoChannel == UNDEFINED_CHANNEL )
		miso_used = false;
	U64 num_frames = GetNumFrames();

   for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

		if( ( frame.mFlags & RC663_ERROR_FLAG ) != 0 )
			continue;
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char mosi_str[128] = "";
		if( mosi_used == true )
			AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, mosi_str, 128 );

		char miso_str[128] = "";
		if( miso_used == true )
			AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, miso_str, 128 );

		U64 packet_id = GetPacketContainingFrameSequential( i ); 
		if( packet_id != INVALID_RESULT_INDEX )
			ss << time_str << "," << packet_id << "," << mosi_str << "," << miso_str << std::endl;
		else
			ss << time_str << ",," << mosi_str << "," << miso_str << std::endl;  //it's ok for a frame not to be included in a packet.
	
		AnalyzerHelpers::AppendToFile( (U8*)ss.str().c_str(), (U32)ss.str().length(), f );
		ss.str( std::string() );
							
		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			AnalyzerHelpers::EndFile( f );
			return;
		}
	}

	UpdateExportProgressAndCheckForCancel( num_frames, num_frames );
	AnalyzerHelpers::EndFile( f );
}

/*****************************************************************************/

void RC663AnalyzerResults::PrintEntryOfExportRegister(void* f, DisplayBase display_base, RC663Analyzer *analyzer, S64 startingSampleInclusive, S64 endingSampleInclusive, S64 prevEndingSampleInclusive, U64 reg, bool isWriteProcess, U64 *payload, U64 lenPayload )
{
	std::stringstream ss;

	U64 trigger_sample = analyzer->GetTriggerSample();
	U32 sample_rate    = analyzer->GetSampleRate();

   U64 time_len   = 1000 * (endingSampleInclusive - startingSampleInclusive);
   U64 time_diff  = 1000 * (startingSampleInclusive - prevEndingSampleInclusive);

	char instr_str[128];
	AnalyzerHelpers::GetNumberString( reg, display_base, 8, instr_str, 128 );

   char start_time_str[128];
   char end_time_str[128];
   char time_len_str[128];
   char time_diff_str[128];

   AnalyzerHelpers::GetTimeString( startingSampleInclusive, trigger_sample, sample_rate, start_time_str, 128 );
   AnalyzerHelpers::GetTimeString( endingSampleInclusive, trigger_sample, sample_rate, end_time_str, 128 );
   AnalyzerHelpers::GetTimeString( time_len, trigger_sample, sample_rate, time_len_str, 128 );
   AnalyzerHelpers::GetTimeString( time_diff, trigger_sample, sample_rate, time_diff_str, 128 );

   replaceDotsWithComma(start_time_str);
   replaceDotsWithComma(end_time_str);
   replaceDotsWithComma(time_len_str);
   replaceDotsWithComma(time_diff_str);

   // print times ...
   ss << start_time_str << ";" << end_time_str << ";" << time_len_str << ";" << time_diff_str << ";";

   if( isWriteProcess )
		ss << "write;";
	else
		ss << "read;";

   if( reg >= NUM_REGISTERS )
	{
		ss << "Unknown register (";
		ss << instr_str;
		ss << ");";
	}
	else
	{
      ss << RC663Registers[reg].long_text << ";";
	}
   ss << lenPayload << ";";

   for( U64 i=0; i < lenPayload; i++)
   {
	   char value[128];
	   AnalyzerHelpers::GetNumberString( *payload, display_base, 8, value, 128 );
      ss << value << ";";
      ++payload;
   }
   ss << std::endl;

   AnalyzerHelpers::AppendToFile( (U8*)ss.str().c_str(), (U32)ss.str().length(), f );
	ss.str( std::string() );
}

/*****************************************************************************/

#define MAX_LEN_PAYLOAD  256

void RC663AnalyzerResults::ExportRegistersToFile( const char* file, DisplayBase display_base )
{
	std::stringstream ss;
	void* f = AnalyzerHelpers::StartFile( file );

	ss << "start Time [s];end Time [s];Time len[ms];Time diff[ms];r/w;register;len payload;payload" << std::endl;

   AnalyzerHelpers::AppendToFile( (U8*)ss.str().c_str(), (U32)ss.str().length(), f );
	ss.str( std::string() );

   U64 num_frames = GetNumFrames();
   bool frameToBePrinted = false;
   bool isWriteProcess   = false;
   S64  startingSampleInclusive = 0;
   S64  endingSampleInclusive = 0;
   S64  prevEndingSampleInclusive = 0;
   U64  reg = 0;
   U64  payload[MAX_LEN_PAYLOAD] = {0};
   U64  lenPayload = 0;

   for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

		if( ( frame.mFlags & RC663_ERROR_FLAG ) != 0 )
			continue;
		
      if( frame.mFlags & RC663_REGISTER_FLAG )
	   {
	   }
	   else
	   {
   	   if( frame.mFlags & RC663_NEW_FRAME_FLAG )
		   {
            if( frameToBePrinted )
            {
               // frame to be printed
               PrintEntryOfExportRegister( f, display_base, mAnalyzer, startingSampleInclusive, endingSampleInclusive, prevEndingSampleInclusive, reg, isWriteProcess, payload, lenPayload );
            }
            lenPayload = 0;
            isWriteProcess = (frame.mFlags & RC663_WRITE_PROCESS_FLAG) ? true : false;
		      reg = frame.mData1 >> 1;
            startingSampleInclusive = frame.mStartingSampleInclusive;

            if( prevEndingSampleInclusive == 0 )
               prevEndingSampleInclusive = startingSampleInclusive;
            else
               prevEndingSampleInclusive = endingSampleInclusive;
		   }
		   else
		   {
	   	   if( frame.mFlags & RC663_WRITE_PROCESS_FLAG )
               payload[lenPayload++] = frame.mData1;
            else
               payload[lenPayload++] = frame.mData2;
		   }
         endingSampleInclusive = frame.mEndingSampleInclusive;
	   }
							
		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			AnalyzerHelpers::EndFile( f );
			return;
		}
      frameToBePrinted = true;
	}
   if( frameToBePrinted )
   {
      // frame to be printed
      PrintEntryOfExportRegister( f, display_base, mAnalyzer, startingSampleInclusive, endingSampleInclusive, prevEndingSampleInclusive, reg, isWriteProcess, payload, lenPayload );
   }

	UpdateExportProgressAndCheckForCancel( num_frames, num_frames );
	AnalyzerHelpers::EndFile( f );
}

/*****************************************************************************/

void RC663AnalyzerResults::ExportFramesToFile( const char* file, DisplayBase display_base )
{
	std::stringstream ss;
	void* f = AnalyzerHelpers::StartFile( file );

   U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate    = mAnalyzer->GetSampleRate();

   ss << "index;start Time [s];end Time [s];r/w;mosi;miso" << std::endl;

   U64 num_frames = GetNumFrames();
   char instr_str[128];
   char start_time_str[128];
   char end_time_str[128];

   bool frameToBePrinted = false;
   bool isWriteProcess   = false;
   S64  startingSampleInclusive = 0;
   S64  endingSampleInclusive = 0;
   U64  reg = 0;
   U64  payload[MAX_LEN_PAYLOAD] = {0};
   U64  lenPayload = 0;

   for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

		if( ( frame.mFlags & RC663_ERROR_FLAG ) != 0 )
			continue;

      ss << i << ";";

	   AnalyzerHelpers::GetNumberString( reg, display_base, 8, instr_str, 128 );

      char* pch;
      AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, start_time_str, 128 );
      AnalyzerHelpers::GetTimeString( frame.mEndingSampleInclusive, trigger_sample, sample_rate, end_time_str, 128 );

      pch=strchr(start_time_str ,'.');
      if(pch) *pch = ',';

      pch=strchr(end_time_str ,'.');
      if(pch) *pch = ',';

      ss << start_time_str << ";";
      ss << end_time_str << ";";

      if( frame.mFlags & RC663_WRITE_PROCESS_FLAG )
         ss << "write;";
      else
         ss << "read;";

	   char mosi[128];
	   char miso[128];
      AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, mosi, 128 );
      AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, miso, 128 );
      ss << mosi << ";";
      ss << miso << ";";
	   if( frame.mFlags & RC663_REGISTER_FLAG )
         ss << "RC663_REGISTER_FLAG - ";
 	   if( frame.mFlags & RC663_NEW_FRAME_FLAG )
         ss << "RC663_NEW_FRAME_FLAG  - ";
      ss << std::endl;

      AnalyzerHelpers::AppendToFile( (U8*)ss.str().c_str(), (U32)ss.str().length(), f );
	   ss.str( std::string() );

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			AnalyzerHelpers::EndFile( f );
			return;
		}
	}

	UpdateExportProgressAndCheckForCancel( num_frames, num_frames );
	AnalyzerHelpers::EndFile( f );
}

/*****************************************************************************/

void RC663AnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
   switch( export_type_user_id)
   {
      case 0:
         // export standard signals
         ExportSignalsToFile( file, display_base);
         break;

      case 1:
         // export read/write of registers
         ExportRegistersToFile( file, display_base);
         break;

      case 2:
         // export frames
         ExportFramesToFile( file, display_base);
         break;

      default:
         break;
   }
}

/*****************************************************************************/

void RC663AnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
   ClearTabularText();

   Frame frame = GetFrame( frame_index );

	std::stringstream ss;

	if( frame.mFlags & RC663_REGISTER_FLAG  )
	{
		char instr_str[128];
		U64 reg = frame.mData1 >> 1;
		AnalyzerHelpers::GetNumberString( reg, display_base, 8, instr_str, 128 );

      if (reg >= NUM_REGISTERS)
		{
			ss << "Unknown register (";
			ss << instr_str;
			ss << ")";
		}
		else
		{
         if(frame.mData1 & 0x01)
				ss << "read ";
			else
				ss << "write ";

         ss << RC663Registers[reg].short_text;
			ss << " (" << instr_str << ")";
		}
	}
	else
	{
		char mosi_str[128];
		char miso_str[128];
		
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, mosi_str, 128 );
		AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, miso_str, 128 );

		if( frame.mFlags & RC663_NEW_FRAME_FLAG )
		{
			mCnt = 0;
			ss << "Register: ";
			ss << mosi_str;
		}
		else
		{
			ss << "Payload[";
			ss << mCnt-1;
			ss << "]: ";
	   	if( frame.mFlags & RC663_WRITE_PROCESS_FLAG )
   			ss << mosi_str;
         else
            ss << miso_str;

         if (mOldFrameId < frame_index)
			{
				mCnt++;
				mOldFrameId = frame_index;
			}
		}
	}

	AddTabularText( ss.str().c_str() );
}

/*****************************************************************************/

void RC663AnalyzerResults::GeneratePacketTabularText( U64 /*packet_id*/, DisplayBase /*display_base*/ )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString( "not supported" );
}

/*****************************************************************************/

void RC663AnalyzerResults::GenerateTransactionTabularText( U64 /*transaction_id*/, DisplayBase /*display_base*/ )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString( "not supported" );
}
